#!/usr/bin/env python3
# Scratch-tree-only patch 5:
#  - SIF MSFLAG/SMFLAG hardware write semantics in sceSifSetReg (EE writes set MSFLAG bits / clear SMFLAG bits)
#  - RESET_CMD (0x80000003): HLE IOP reboot, completed lazily at the EE's first SMFLAG poll (BOOTEND etc.)
#  - syscall 0x6B (SifStopDma) routed to the existing stub
#  - LOADFILE RPC (sid 0x80000006, fno 0 = LF_F_MOD_LOAD) served through runtime->loadIopModule
import sys

ROOT = "/tmp/ps2recomp_build/PS2Recomp/ps2xRuntime"
P = ROOT + "/src/lib/Kernel/Stubs/SIF.cpp"
s = open(P).read()


def rep(old, new, path=None):
    global s
    n = s.count(old)
    if n != 1:
        print(f"FAIL: expected 1 match, found {n} for:\n{old[:160]}")
        sys.exit(1)
    s = s.replace(old, new)


# --- state
rep("""        std::deque<SifPendingReply> g_sifPendingReplies;
""", """        std::deque<SifPendingReply> g_sifPendingReplies;
        // Set by RESET_CMD; the reboot is completed (IOP reset + SMFLAG boot bits) at the EE's next SMFLAG poll,
        // i.e. after the raw sceSifResetIop finished clearing SIFINIT/CMDINIT/BOOTEND and the kernel registers.
        bool g_sifIopRebootPending = false;
        constexpr uint32_t kSifRegMsFlag = 0x3u;
        constexpr uint32_t kSifRegSmFlag = 0x4u;
        constexpr uint32_t kSifStatSifInit = 0x00010000u;
        constexpr uint32_t kSifStatCmdInit = 0x00020000u;
        constexpr uint32_t kSifStatBootEnd = 0x00040000u;
""")
rep("""            g_sifPendingReplies.clear();

            g_sifRegs[kSifRegBootStatus] = kSifBootReadyMask;""",
    """            g_sifPendingReplies.clear();
            g_sifIopRebootPending = false;

            g_sifRegs[kSifRegBootStatus] = kSifBootReadyMask;""")

# --- RESET_CMD + LOADFILE handlers (before hleIopSifCommands)
rep("""        void hleIopSifCommands(uint8_t *rdram,
                               R5900Context *ctx,
                               PS2Runtime *runtime,
                               const std::vector<Ps2SifDmaTransfer> &transfers,
                               const std::vector<std::vector<uint8_t>> &payloads)
        {""", """        // SIF_CMD_RESET_CMD (0x80000003): _sif_cmd_reset_pkt { hdr; int arglen; int mode; char arg[80] }.
        // The IOP reboots (rom0:UDNL with the given image). The EE's sceSifResetIop clears SMFLAG bits and the
        // kernel registers right after sending this, so the "boot finished" state is published lazily.
        void hleIopSifReset(const uint8_t *packet, uint32_t size)
        {
            std::string arg;
            if (size > 0x18u)
            {
                const uint32_t argLen = std::min<uint32_t>(readPacketU32(packet, 0x10), size - 0x18u);
                arg.assign(reinterpret_cast<const char *>(packet + 0x18u), argLen);
                arg = arg.c_str();
            }
            std::cerr << "[sif-cmd-hle] RESET_CMD arg='" << arg << "' mode=" << readPacketU32(packet, 0x14)
                      << " -> IOP reboot scheduled (HLE: IOP subsystem reset, IOPRP image not loaded)" << std::endl;
            g_sifIopRebootPending = true;
        }

        void completeIopRebootIfPending(PS2Runtime *runtime)
        {
            if (!g_sifIopRebootPending)
                return;
            g_sifIopRebootPending = false;
            PS2IopTransport::reset(runtime);
            g_sifRpcServersBySid.clear();
            g_sifRpcSidByHandle.clear();
            g_sifPendingReplies.clear();
            g_sifRegs[kSifRegSmFlag] |= kSifStatSifInit | kSifStatCmdInit | kSifStatBootEnd;
            std::cerr << "[sif-cmd-hle] IOP reboot complete: SMFLAG=0x" << std::hex << g_sifRegs[kSifRegSmFlag] << std::dec << std::endl;
        }

        // LOADFILE (rom0:LOADFILE, sid 0x80000006). fno 0 = LF_F_MOD_LOAD:
        //   send  _lf_module_load_arg { int arg_len; int modres; char path[252]; char args[252] }
        //   recv  { int result (module id or error); int modres (start result) }
        // Served through the runtime's own IOP module loader (HLE module or IRX on the R3000 core).
        bool hleIopLoadfile(uint8_t *rdram, PS2Runtime *runtime, uint32_t fno, uint32_t sendAddress, uint32_t sendSize,
                            uint32_t receive, uint32_t receiveSize)
        {
            constexpr uint32_t kLfPathMax = 252u;
            if (fno != 0u || !runtime || sendAddress == 0u || sendSize < 8u + kLfPathMax)
                return false;
            std::vector<uint8_t> arg(std::min<uint32_t>(sendSize, 8u + 2u * kLfPathMax));
            if (!readEeRange(rdram, sendAddress, arg.data(), static_cast<uint32_t>(arg.size())))
                return false;
            const uint32_t argLen = readPacketU32(arg.data(), 0u);
            std::string path(reinterpret_cast<const char *>(arg.data() + 8u), kLfPathMax);
            path = path.c_str();
            const uint8_t *args = arg.size() >= 8u + 2u * kLfPathMax ? arg.data() + 8u + kLfPathMax : nullptr;
            const uint32_t argBytes = args ? std::min<uint32_t>(argLen, kLfPathMax) : 0u;
            const ps2x::iop::ModuleLoadResult loaded = runtime->loadIopModule(path, args, argBytes);
            std::cerr << "[sif-rpc-hle] LOADFILE load '" << path << "' arglen=" << argLen
                      << " -> handled=" << loaded.handled << " id=" << loaded.moduleId << " start=" << loaded.startResult << std::endl;
            int32_t reply[2] = {loaded.handled ? loaded.moduleId : -1, loaded.startResult};
            if (receive != 0u && receiveSize >= sizeof(reply))
                (void)writeEeRange(rdram, receive, reply, sizeof(reply));
            return true;
        }

        void hleIopSifCommands(uint8_t *rdram,
                               R5900Context *ctx,
                               PS2Runtime *runtime,
                               const std::vector<Ps2SifDmaTransfer> &transfers,
                               const std::vector<std::vector<uint8_t>> &payloads)
        {""")
rep("""                case 0x80000002u:
                    hleIopSifCmdInit(rdram, payload.data(), static_cast<uint32_t>(payload.size()));
                    break;
                case kSifCmdRpcBind:""", """                case 0x80000002u:
                    hleIopSifCmdInit(rdram, payload.data(), static_cast<uint32_t>(payload.size()));
                    break;
                case 0x80000003u:
                    hleIopSifReset(payload.data(), static_cast<uint32_t>(payload.size()));
                    break;
                case kSifCmdRpcBind:""")

# --- hook LOADFILE into the RPC call path (before the generic HLE service dispatch); LOADFILE is a rom0 module
rep("""            bool handled = false;
            uint32_t resultAddress = 0u;
            if (server && server->hle)
            {""", """            bool handled = false;
            uint32_t resultAddress = 0u;
            constexpr uint32_t kSidLoadfile = 0x80000006u;
            if (sid == kSidLoadfile)
            {
                handled = hleIopLoadfile(rdram, runtime, fno, sendAddress, sendSize, receive, receiveSize);
            }
            else if (server && server->hle)
            {""")
# rom0 LOADFILE is always "present": mark it bindable as an HLE route in ensureSifRpcServer
rep("""            server.hle = PS2IopTransport::canBindRpc(runtime, sid);""",
    """            server.hle = sid == 0x80000006u /* rom0:LOADFILE (bridge-served) */ || PS2IopTransport::canBindRpc(runtime, sid);""")

# --- SetReg: MSFLAG set-bits / SMFLAG clear-bits (EE side of the SBUS flag registers)
rep("""            g_sifRegs[reg] = value;
            shouldLog = shouldTraceSifReg(reg) && g_sifSetRegLogCount < 128u;""",
    """            if (reg == kSifRegMsFlag)
                g_sifRegs[reg] = prev | value;      // EE writes set MSFLAG bits
            else if (reg == kSifRegSmFlag)
                g_sifRegs[reg] = prev & ~value;     // EE writes clear SMFLAG bits (IOP sets them)
            else
                g_sifRegs[reg] = value;
            shouldLog = shouldTraceSifReg(reg) && g_sifSetRegLogCount < 128u;""")

# --- GetReg: finish a pending IOP reboot at the first SMFLAG poll
rep("""    void sceSifGetReg(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        flushPendingSifReply(rdram, runtime);
        const uint32_t reg = getRegU32(ctx, 4);""", """    void sceSifGetReg(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        flushPendingSifReply(rdram, runtime);
        const uint32_t reg = getRegU32(ctx, 4);
        if (reg == kSifRegSmFlag)
            completeIopRebootIfPending(runtime);""")

open(P, "w").write(s)
print("patched", P)

# --- Dispatcher: syscall 0x6B SifStopDma
D = ROOT + "/src/lib/Kernel/Syscalls/Dispatcher.cpp"
d = open(D).read()
old = """        case 0x64:
            FlushCache(rdram, ctx, runtime);
            return true;
"""
new = """        case 0x64:
            FlushCache(rdram, ctx, runtime);
            return true;
        case 0x6B: // SifStopDma
            ps2_stubs::sceSifStopDma(rdram, ctx, runtime);
            return true;
"""
assert d.count(old) == 1, "dispatcher anchor"
open(D, "w").write(d.replace(old, new))
print("patched", D)
