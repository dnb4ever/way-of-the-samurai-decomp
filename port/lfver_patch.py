#!/usr/bin/env python3
# Scratch-tree-only patch 6: LOADFILE fno 0xFF = version query; answer with the tag of the IOPRP image the
# game rebooted with (IOPRPxyz.IMG -> "xyz0"), which the EE loader (func_00115FB8) compares before loading.
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


rep("""        bool g_sifIopRebootPending = false;
""", """        bool g_sifIopRebootPending = false;
        // Version tag of the LOADFILE module in service, as reported by its fno 0xFF query. Derived from the
        // IOPRP image named in the last RESET_CMD (IOPRPxyz.IMG -> "xyz0"); wildcard before any reboot.
        std::string g_sifLoadfileVersionTag = "....";
""")
rep("""            std::cerr << "[sif-cmd-hle] RESET_CMD arg='" << arg << "' mode=" << readPacketU32(packet, 0x14)""",
    """            const size_t iop = arg.find("IOPRP");
            if (iop != std::string::npos && iop + 8u <= arg.size() &&
                std::isdigit(static_cast<unsigned char>(arg[iop + 5])) &&
                std::isdigit(static_cast<unsigned char>(arg[iop + 6])) &&
                std::isdigit(static_cast<unsigned char>(arg[iop + 7])))
            {
                g_sifLoadfileVersionTag = arg.substr(iop + 5u, 3u) + "0";
            }
            std::cerr << "[sif-cmd-hle] RESET_CMD arg='" << arg << "' mode=" << readPacketU32(packet, 0x14)""")
rep("""            constexpr uint32_t kLfPathMax = 252u;
            if (fno != 0u || !runtime || sendAddress == 0u || sendSize < 8u + kLfPathMax)
                return false;""", """            constexpr uint32_t kLfPathMax = 252u;
            if (fno == 0xFFu)
            {
                // Version query: the EE loader compares the 4-byte tag with the IOPRP it rebooted with.
                char tag[4] = {'.', '.', '.', '.'};
                std::memcpy(tag, g_sifLoadfileVersionTag.c_str(), std::min<size_t>(4u, g_sifLoadfileVersionTag.size()));
                if (receive != 0u && receiveSize >= sizeof(tag))
                    (void)writeEeRange(rdram, receive, tag, sizeof(tag));
                std::cerr << "[sif-rpc-hle] LOADFILE version query -> '" << std::string(tag, 4) << "'" << std::endl;
                return true;
            }
            if (fno != 0u || !runtime || sendAddress == 0u || sendSize < 8u + kLfPathMax)
                return false;""")
open(P, "w").write(s)
print("patched", P)
