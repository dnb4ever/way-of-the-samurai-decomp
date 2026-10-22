#!/usr/bin/env python3
# Scratch-tree-only patch: raw-SIFRPC -> IOP-HLE bridge inside the sceSifSetDma HLE (Stubs/SIF.cpp).
# Emulates the IOP side of SIFRPC (rom0:SIFCMD rpc_bind/rpc_call/rpc_rdata handlers) for a game whose
# EE libkernel SIFRPC layer is recompiled raw: requests are parsed off the EE->IOP SIF command packets,
# serviced through the runtime's existing IOP RPC HLE (handleIopRpc), and answered with RPC_END packets
# delivered into the EE's SIFCMD receive buffer so the game's own RPC_END handler completes the call.
import sys

P = "/tmp/ps2recomp_build/PS2Recomp/ps2xRuntime/src/lib/Kernel/Stubs/SIF.cpp"
s = open(P).read()


def rep(old, new):
    global s
    n = s.count(old)
    if n != 1:
        print(f"FAIL: expected 1 match, found {n} for:\n{old[:160]}")
        sys.exit(1)
    s = s.replace(old, new)


# 0) includes
rep('#include "Common.h"\n', '#include "Common.h"\n#include <deque>\n#include <set>\n')

# 1) state (+ reset)
rep("""        // EE packet-receive buffer announced by the raw libkernel sceSifInitCmd (INIT_CMD opt=0).
        uint32_t g_sifEeCmdPktBuf = 0u;
""", """        // EE packet-receive buffer announced by the raw libkernel sceSifInitCmd (INIT_CMD opt=0).
        uint32_t g_sifEeCmdPktBuf = 0u;

        // Raw-SIFRPC bridge state (EE executor thread only).
        struct SifRpcHleServer
        {
            uint32_t sid = 0u;
            uint32_t handle = 0u; // pseudo IOP server descriptor address (what the EE sees as client->server)
            uint32_t buff = 0u;   // IOP receive buffer the EE DMAs call arguments into
            uint32_t cbuff = 0u;
            bool hle = false;     // an IOP HLE service answers this sid
            uint32_t calls = 0u;
        };
        std::unordered_map<uint32_t, SifRpcHleServer> g_sifRpcServersBySid;
        std::unordered_map<uint32_t, uint32_t> g_sifRpcSidByHandle;
        std::set<std::pair<uint32_t, uint32_t>> g_sifRpcUnimplementedLogged;
        struct SifPendingReply
        {
            uint32_t dest = 0u;
            std::array<uint8_t, 0x30> packet{};
        };
        std::deque<SifPendingReply> g_sifPendingReplies;
""")
rep("""            g_sifEeCmdPktBuf = 0u;

            g_sifRegs[kSifRegBootStatus] = kSifBootReadyMask;""",
    """            g_sifEeCmdPktBuf = 0u;
            g_sifRpcServersBySid.clear();
            g_sifRpcSidByHandle.clear();
            g_sifRpcUnimplementedLogged.clear();
            g_sifPendingReplies.clear();

            g_sifRegs[kSifRegBootStatus] = kSifBootReadyMask;""")

# 2) bridge implementation, appended after hleIopSifCmdInit (still inside the anonymous namespace)
rep("""            std::cerr << "[sif-cmd-hle] INIT_CMD opt=1: SET_SREG(0,1) reply placed at 0x" << std::hex << dest << std::dec << std::endl;
        }
    }
""", """            std::cerr << "[sif-cmd-hle] INIT_CMD opt=1: SET_SREG(0,1) reply placed at 0x" << std::hex << dest << std::dec << std::endl;
        }

        // ------------------------------------------------------------------------------------------
        // Raw-SIFRPC -> IOP-HLE bridge.
        //
        // Wire formats follow ps2sdk ee/kernel sifrpc (SifCmdHeader {u8 psize; u8 dsize[3]; u32 dest;
        // u32 cid; u32 opt}, then rec_id@+0x10, pkt_addr@+0x14, rpc_id@+0x18, client@+0x1C). The game's
        // RPC_END handler (0x1118C0) consumes cid@+0x20, server@+0x24, buff@+0x28, cbuff@+0x2C and then
        // signals client->sema / calls client->end_function itself, so callbacks and semaphores are never
        // touched from the host side. The IOP only ever has one outstanding packet in the EE receive
        // buffer; replies are queued while psize != 0 and flushed when the game re-arms SIF0 (SetDChain).
        // ------------------------------------------------------------------------------------------
        constexpr uint32_t kSifCmdRpcEnd = 0x80000008u;
        constexpr uint32_t kSifCmdRpcBind = 0x80000009u;
        constexpr uint32_t kSifCmdRpcCall = 0x8000000Au;
        constexpr uint32_t kSifCmdRpcRdata = 0x8000000Cu;
        constexpr uint32_t kSifRpcHleServerBufferBytes = 0x10000u;
        constexpr uint32_t kSifRpcHleServerCbufferBytes = 0x400u;

        uint32_t readPacketU32(const uint8_t *packet, uint32_t offset)
        {
            uint32_t value = 0u;
            std::memcpy(&value, packet + offset, sizeof(value));
            return value;
        }

        bool eeCmdPktBufFree(const uint8_t *rdram)
        {
            if (g_sifEeCmdPktBuf == 0u)
                return false;
            const uint8_t *psize = getConstMemPtr(const_cast<uint8_t *>(rdram), g_sifEeCmdPktBuf);
            return psize && *psize == 0u;
        }

        void deliverSifReply(uint8_t *rdram, const std::array<uint8_t, 0x30> &packet)
        {
            if (g_sifEeCmdPktBuf == 0u)
            {
                std::cerr << "[sif-rpc-hle] EE pktbuf unknown; dropping RPC_END" << std::endl;
                return;
            }
            if (eeCmdPktBufFree(rdram) && g_sifPendingReplies.empty())
            {
                if (!writeEeRange(rdram, g_sifEeCmdPktBuf, packet.data(), static_cast<uint32_t>(packet.size())))
                    std::cerr << "[sif-rpc-hle] failed to write RPC_END at 0x" << std::hex << g_sifEeCmdPktBuf << std::dec << std::endl;
                return;
            }
            g_sifPendingReplies.push_back(SifPendingReply{g_sifEeCmdPktBuf, packet});
        }

        // Returns true when a queued reply was placed (caller must raise DMAC ch5 for it).
        bool placePendingSifReply(uint8_t *rdram)
        {
            if (g_sifPendingReplies.empty() || !eeCmdPktBufFree(rdram))
                return false;
            const SifPendingReply reply = g_sifPendingReplies.front();
            g_sifPendingReplies.pop_front();
            if (!writeEeRange(rdram, reply.dest, reply.packet.data(), static_cast<uint32_t>(reply.packet.size())))
            {
                std::cerr << "[sif-rpc-hle] failed to write queued RPC_END at 0x" << std::hex << reply.dest << std::dec << std::endl;
                return false;
            }
            return true;
        }

        std::array<uint8_t, 0x30> makeRpcEndPacket(const uint8_t *request, uint32_t cid, uint32_t server, uint32_t buff, uint32_t cbuff)
        {
            std::array<uint8_t, 0x30> packet{};
            uint32_t words[12] = {
                0x30u, 0u, kSifCmdRpcEnd, 0u,
                readPacketU32(request, 0x10), readPacketU32(request, 0x14),
                readPacketU32(request, 0x18), readPacketU32(request, 0x1C),
                cid, server, buff, cbuff};
            std::memcpy(packet.data(), words, sizeof(words));
            return packet;
        }

        SifRpcHleServer &ensureSifRpcServer(PS2Runtime *runtime, uint32_t sid)
        {
            auto it = g_sifRpcServersBySid.find(sid);
            if (it != g_sifRpcServersBySid.end())
                return it->second;
            SifRpcHleServer server{};
            server.sid = sid;
            server.hle = PS2IopTransport::canBindRpc(runtime, sid);
            server.handle = runtime ? runtime->allocateIopMemory(0x40u, 16u) : 0u;
            server.buff = runtime ? runtime->allocateIopMemory(kSifRpcHleServerBufferBytes, 64u) : 0u;
            server.cbuff = runtime ? runtime->allocateIopMemory(kSifRpcHleServerCbufferBytes, 64u) : 0u;
            if (server.handle != 0u)
                g_sifRpcSidByHandle[server.handle] = sid;
            std::cerr << "[sif-rpc-hle] bind sid=0x" << std::hex << sid
                      << (server.hle ? " -> IOP HLE service" : " -> NO IOP service (pseudo server, calls get empty replies)")
                      << " server=0x" << server.handle << " buff=0x" << server.buff << std::dec << std::endl;
            return g_sifRpcServersBySid.emplace(sid, server).first->second;
        }

        void hleIopSifRpcBind(uint8_t *rdram, PS2Runtime *runtime, const uint8_t *packet)
        {
            const uint32_t sid = readPacketU32(packet, 0x20);
            SifRpcHleServer &server = ensureSifRpcServer(runtime, sid);
            if (server.handle == 0u)
                std::cerr << "[sif-rpc-hle] bind sid=0x" << std::hex << sid << ": IOP memory exhausted, answering server=0" << std::dec << std::endl;
            deliverSifReply(rdram, makeRpcEndPacket(packet, kSifCmdRpcBind, server.handle, server.buff, server.cbuff));
        }

        bool copyEeToEe(uint8_t *rdram, uint32_t dest, uint32_t src, uint32_t size)
        {
            std::vector<uint8_t> bytes(size);
            return readEeRange(rdram, src, bytes.data(), size) && writeEeRange(rdram, dest, bytes.data(), size);
        }

        void zeroEe(uint8_t *rdram, uint32_t dest, uint32_t size)
        {
            const std::vector<uint8_t> zeros(size, 0u);
            (void)writeEeRange(rdram, dest, zeros.data(), size);
        }

        void hleIopSifRpcCall(uint8_t *rdram,
                              R5900Context *ctx,
                              PS2Runtime *runtime,
                              const uint8_t *packet,
                              const std::vector<Ps2SifDmaTransfer> &transfers)
        {
            const uint32_t client = readPacketU32(packet, 0x1C);
            const uint32_t fno = readPacketU32(packet, 0x20);
            const uint32_t sendSize = readPacketU32(packet, 0x24);
            const uint32_t receive = readPacketU32(packet, 0x28);
            const uint32_t receiveSize = readPacketU32(packet, 0x2C);
            const uint32_t rmode = readPacketU32(packet, 0x30);
            const uint32_t serverHandle = readPacketU32(packet, 0x34);

            SifRpcHleServer *server = nullptr;
            {
                const auto sidIt = g_sifRpcSidByHandle.find(serverHandle);
                if (sidIt != g_sifRpcSidByHandle.end())
                    server = &g_sifRpcServersBySid[sidIt->second];
            }
            const uint32_t sid = server ? server->sid : 0u;
            if (server)
                ++server->calls;

            // The call arguments travel as a separate descriptor in the same sceSifSetDma list, aimed at
            // the header's IOP dest. Its EE source is what the HLE services read from.
            const uint32_t headerDest = readPacketU32(packet, 0x4);
            uint32_t sendAddress = 0u;
            for (const Ps2SifDmaTransfer &xfer : transfers)
            {
                if (headerDest != 0u && xfer.dest == headerDest && xfer.size > 0)
                {
                    sendAddress = xfer.src;
                    break;
                }
            }

            bool handled = false;
            uint32_t resultAddress = 0u;
            if (server && server->hle)
            {
                ps2x::iop::RpcRequest request{};
                request.clientAddress = client;
                request.serverAddress = serverHandle;
                request.serverBuffer = server->buff;
                request.sid = sid;
                request.function = fno;
                request.mode = rmode == 0u ? 1u : 0u; // SIF_RPC_M_NOWAIT only when no completion is expected
                request.send = {sendAddress, sendSize};
                request.receive = {receive, receiveSize};
                const ps2x::iop::RpcResult result = PS2IopTransport::handleRpc(runtime, rdram, ctx, request);
                handled = result.handled;
                resultAddress = result.resultAddress;
                if (result.guestFunction != 0u)
                    std::cerr << "[sif-rpc-hle] sid=0x" << std::hex << sid << " fno=" << fno
                              << " asked for EE-side dispatch 0x" << result.guestFunction << " (not applicable to raw SIFRPC)" << std::dec << std::endl;
            }

            if (receive != 0u && receiveSize != 0u)
            {
                if (handled && resultAddress != 0u && resultAddress != receive)
                    (void)copyEeToEe(rdram, receive, resultAddress, receiveSize);
                else if (!handled)
                    zeroEe(rdram, receive, receiveSize);
            }
            if (!handled && g_sifRpcUnimplementedLogged.insert({sid, fno}).second)
            {
                std::cerr << "[sif-rpc-hle] unimplemented service sid=0x" << std::hex << sid << " fno=0x" << fno
                          << " send=" << std::dec << sendSize << " recv=" << receiveSize << " -> empty reply" << std::endl;
            }

            // rmode == 0 means the caller asked for SIF_RPC_M_NOWAIT without an end function: the IOP
            // sends no RPC_END for those.
            if (rmode != 0u)
                deliverSifReply(rdram, makeRpcEndPacket(packet, kSifCmdRpcCall, serverHandle,
                                                        server ? server->buff : 0u, server ? server->cbuff : 0u));
        }

        void hleIopSifRpcRdata(uint8_t *rdram, PS2Runtime *runtime, const uint8_t *packet)
        {
            const uint32_t src = readPacketU32(packet, 0x20);  // IOP address
            const uint32_t dest = readPacketU32(packet, 0x24); // EE address
            const uint32_t size = readPacketU32(packet, 0x28);
            if (dest != 0u && size != 0u && size <= PS2_RAM_SIZE)
            {
                std::vector<uint8_t> bytes(size, 0u);
                if (!runtime || !runtime->readIopMemory(src, bytes.data(), size))
                    std::cerr << "[sif-rpc-hle] RPC_RDATA: unreadable IOP range 0x" << std::hex << src << "+" << size << ", zero-filling" << std::dec << std::endl;
                (void)writeEeRange(rdram, dest, bytes.data(), size);
            }
            deliverSifReply(rdram, makeRpcEndPacket(packet, kSifCmdRpcRdata, 0u, 0u, 0u));
        }

        void hleIopSifCommands(uint8_t *rdram,
                               R5900Context *ctx,
                               PS2Runtime *runtime,
                               const std::vector<Ps2SifDmaTransfer> &transfers,
                               const std::vector<std::vector<uint8_t>> &payloads)
        {
            for (size_t i = 0; i < payloads.size(); ++i)
            {
                const std::vector<uint8_t> &payload = payloads[i];
                if (payload.size() < 16u)
                    continue;
                const uint32_t psize = payload[0];
                if (psize < 16u || psize > payload.size())
                    continue; // not a SIF command packet (plain data descriptor)
                const uint32_t cid = readPacketU32(payload.data(), 8u);
                switch (cid)
                {
                case 0x80000002u:
                    hleIopSifCmdInit(rdram, payload.data(), static_cast<uint32_t>(payload.size()));
                    break;
                case kSifCmdRpcBind:
                    if (payload.size() >= 0x24u)
                        hleIopSifRpcBind(rdram, runtime, payload.data());
                    break;
                case kSifCmdRpcCall:
                    if (payload.size() >= 0x38u)
                        hleIopSifRpcCall(rdram, ctx, runtime, payload.data(), transfers);
                    break;
                case kSifCmdRpcRdata:
                    if (payload.size() >= 0x2Cu)
                        hleIopSifRpcRdata(rdram, runtime, payload.data());
                    break;
                default:
                {
                    static std::set<uint32_t> loggedCids;
                    if (loggedCids.insert(cid).second)
                        std::cerr << "[sif-rpc-hle] unhandled EE->IOP SIF command cid=0x" << std::hex << cid
                                  << " psize=" << psize << std::dec << " (no IOP-side handler emulated)" << std::endl;
                    break;
                }
                }
            }
        }

        // Called from the SIF syscalls the raw libkernel uses to re-arm / poll the link; places the next
        // queued IOP->EE packet once the receive buffer is free and raises SIF0 (DMAC ch5) for it.
        void flushPendingSifReply(uint8_t *rdram, PS2Runtime *runtime)
        {
            if (placePendingSifReply(rdram) && runtime)
                ps2_syscalls::dispatchDmacHandlersForCause(rdram, runtime, 5u);
        }
    }
""")

# 3) sceSifSetDma: collect payloads, process commands after the whole list is copied
rep("""                const uint32_t sizeBytes = static_cast<uint32_t>(xfer.size);
                std::vector<uint8_t> payload(sizeBytes);
                if (!readEeRange(rdram, xfer.src, payload.data(), sizeBytes) || !runtime->writeIopMemory(xfer.dest, payload.data(), payload.size()))
                {
                    ok = false;
                    break;
                }
                hleIopSifCmdInit(rdram, payload.data(), sizeBytes);
                if (runtime)
                {""", """                const uint32_t sizeBytes = static_cast<uint32_t>(xfer.size);
                std::vector<uint8_t> payload(sizeBytes);
                if (!readEeRange(rdram, xfer.src, payload.data(), sizeBytes) || !runtime->writeIopMemory(xfer.dest, payload.data(), payload.size()))
                {
                    ok = false;
                    break;
                }
                copiedTransfers.push_back(xfer);
                copiedPayloads.push_back(std::move(payload));
                if (runtime)
                {""")
rep("""        if (ok)
        {
            for (uint32_t i = 0; i < pendingCount; ++i)
            {
                const Ps2SifDmaTransfer &xfer = pending[i];""", """        std::vector<Ps2SifDmaTransfer> copiedTransfers;
        std::vector<std::vector<uint8_t>> copiedPayloads;
        if (ok)
        {
            for (uint32_t i = 0; i < pendingCount; ++i)
            {
                const Ps2SifDmaTransfer &xfer = pending[i];""")
rep("""        ps2_syscalls::dispatchDmacHandlersForCause(rdram, runtime, 5u);

        setReturnS32(ctx, static_cast<int32_t>(allocateSifDmaTransferId()));
    }""", """        // Emulated IOP side: consume SIF command packets (INIT_CMD / RPC_BIND / RPC_CALL / RPC_RDATA)
        // and stage the IOP->EE replies before the raw sceSifCmdIntrHdlr is dispatched below.
        (void)placePendingSifReply(rdram);
        hleIopSifCommands(rdram, ctx, runtime, copiedTransfers, copiedPayloads);
        ps2_syscalls::dispatchDmacHandlersForCause(rdram, runtime, 5u);
        setReturnS32(ctx, static_cast<int32_t>(allocateSifDmaTransferId()));
    }""")

# 4) flush points: SetDChain (the raw ISR re-arms SIF0 with it), DmaStat, GetReg
rep("""    void sceSifSetDChain(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        (void)rdram;
        (void)runtime;
        setReturnS32(ctx, 0);
    }""", """    void sceSifSetDChain(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        flushPendingSifReply(rdram, runtime);
        setReturnS32(ctx, 0);
    }""")
rep("""        (void)getRegU32(ctx, 4); // trid

        // Transfers are applied immediately by sceSifSetDma in this runtime.
        setReturnS32(ctx, -1);""", """        (void)getRegU32(ctx, 4); // trid
        flushPendingSifReply(rdram, runtime);

        // Transfers are applied immediately by sceSifSetDma in this runtime.
        setReturnS32(ctx, -1);""")
rep("""    void sceSifGetReg(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const uint32_t reg = getRegU32(ctx, 4);
        uint32_t value = 0u;
        bool shouldLog = false;""", """    void sceSifGetReg(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        flushPendingSifReply(rdram, runtime);
        const uint32_t reg = getRegU32(ctx, 4);
        uint32_t value = 0u;
        bool shouldLog = false;""")

open(P, "w").write(s)
print("patched", P)
