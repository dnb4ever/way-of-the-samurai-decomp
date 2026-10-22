#!/usr/bin/env python3
# Scratch-tree-only: minimal padman (old protocol, rom0:PADMAN 4.x) RPC responder in the SIFRPC bridge.
# libpad sends every request as fno 1 with a 128-byte buffer: word0 = command, reply in the same buffer,
# result in word3. No controller is attached: pad-data areas registered by port-open stay zeroed
# (PAD_STATE_DISCONN); the module version query answers 4.0 so libpad's version check passes.
import shutil
import sys

P = "/tmp/ps2recomp_build/PS2Recomp/ps2xRuntime/src/lib/Kernel/Stubs/SIF.cpp"
shutil.copyfile(P, P + ".fix9")
s = open(P).read()


def rep(old, new):
    global s
    n = s.count(old)
    if n != 1:
        print("FAIL matches", n, old[:100])
        sys.exit(1)
    s = s.replace(old, new)


rep("""        bool hleIopLoadfile(uint8_t *rdram, PS2Runtime *runtime, uint32_t fno, uint32_t sendAddress, uint32_t sendSize,
                            uint32_t receive, uint32_t receiveSize);
""", """        bool hleIopLoadfile(uint8_t *rdram, PS2Runtime *runtime, uint32_t fno, uint32_t sendAddress, uint32_t sendSize,
                            uint32_t receive, uint32_t receiveSize);

        // rom0:PADMAN (old libpad protocol). Every call is fno 1 with a 128-byte request/reply buffer:
        // word0 = command, reply mirrors the request with the result in word3.
        //   0x01 open(port, slot, pad-data address)   0x0E ?set-mode   0x10 init   0x12 get module version
        // No controller is connected: pad-data blocks stay zeroed (PAD_STATE_DISCONN).
        bool hleIopPadman(uint8_t *rdram, uint32_t sid, uint32_t fno, uint32_t sendAddress, uint32_t sendSize,
                          uint32_t receive, uint32_t receiveSize)
        {
            constexpr uint32_t kPadCmdOpen = 0x01u;
            constexpr uint32_t kPadCmdGetModVersion = 0x12u;
            constexpr uint32_t kPadmanVersion = 0x0400u; // "4.0" – matches libpad.a 4.0 in this ELF
            if (fno != 1u || sendAddress == 0u || sendSize < 16u || receive == 0u || receiveSize < 16u)
                return false;
            std::vector<uint8_t> buffer(std::min<uint32_t>(sendSize, receiveSize));
            if (!readEeRange(rdram, sendAddress, buffer.data(), static_cast<uint32_t>(buffer.size())))
                return false;
            const uint32_t command = readPacketU32(buffer.data(), 0u);
            uint32_t result = 1u;
            if (command == kPadCmdGetModVersion)
                result = kPadmanVersion;
            else if (command == kPadCmdOpen)
            {
                const uint32_t port = readPacketU32(buffer.data(), 4u);
                const uint32_t slot = readPacketU32(buffer.data(), 8u);
                const uint32_t padData = readPacketU32(buffer.data(), 16u);
                std::cerr << "[sif-rpc-hle] padman open port=" << port << " slot=" << slot << " padData=0x" << std::hex << padData
                          << std::dec << " (no controller: state stays DISCONN)" << std::endl;
            }
            std::memcpy(buffer.data() + 12u, &result, sizeof(result));
            if (!writeEeRange(rdram, receive, buffer.data(), static_cast<uint32_t>(buffer.size())))
                return false;
            static std::set<std::pair<uint32_t, uint32_t>> logged;
            if (logged.insert({sid, command}).second)
                std::cerr << "[sif-rpc-hle] padman sid=0x" << std::hex << sid << " cmd=0x" << command << " -> result=0x" << result << std::dec << std::endl;
            return true;
        }
""")
rep("""            constexpr uint32_t kSidLoadfile = 0x80000006u;
            if (sid == kSidLoadfile)
            {
                handled = hleIopLoadfile(rdram, runtime, fno, sendAddress, sendSize, receive, receiveSize);
            }""", """            constexpr uint32_t kSidLoadfile = 0x80000006u;
            constexpr uint32_t kSidPadman1 = 0x80000100u;
            constexpr uint32_t kSidPadman2 = 0x80000101u;
            if (sid == kSidLoadfile)
            {
                handled = hleIopLoadfile(rdram, runtime, fno, sendAddress, sendSize, receive, receiveSize);
            }
            else if (sid == kSidPadman1 || sid == kSidPadman2)
            {
                handled = hleIopPadman(rdram, sid, fno, sendAddress, sendSize, receive, receiveSize);
            }""")
rep("""            server.hle = sid == 0x80000006u /* rom0:LOADFILE (bridge-served) */ || PS2IopTransport::canBindRpc(runtime, sid);""",
    """            server.hle = sid == 0x80000006u /* rom0:LOADFILE (bridge-served) */ ||
                         sid == 0x80000100u || sid == 0x80000101u /* rom0:PADMAN (bridge-served) */ ||
                         PS2IopTransport::canBindRpc(runtime, sid);""")
open(P, "w").write(s)
print("patched", P)
