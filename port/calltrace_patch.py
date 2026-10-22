#!/usr/bin/env python3
# Scratch-tree-only: larger RPC call trace with payload previews for non-rom0 services (services map).
import sys

P = "/tmp/ps2recomp_build/PS2Recomp/ps2xRuntime/src/lib/Kernel/Stubs/SIF.cpp"
s = open(P).read()
old = """            {
                static uint32_t s_callLogs = 0u;
                if (s_callLogs < 96u)
                {
                    ++s_callLogs;
                    std::cerr << "[sif-rpc-hle] call sid=0x" << std::hex << sid << " fno=0x" << fno << std::dec
                              << " rmode=" << rmode << " send=" << sendSize << " recv=" << receiveSize
                              << " handled=" << handled << std::endl;
                }
            }"""
new = """            {
                static uint32_t s_callLogs = 0u;
                if (s_callLogs < 4000u)
                {
                    ++s_callLogs;
                    auto preview = [&](uint32_t address, uint32_t size)
                    {
                        std::string out;
                        const uint32_t count = std::min<uint32_t>(size, 16u);
                        for (uint32_t i = 0; i < count; ++i)
                        {
                            const uint8_t *byte = address ? getConstMemPtr(rdram, address + i) : nullptr;
                            char buf[4];
                            std::snprintf(buf, sizeof(buf), "%02x", byte ? *byte : 0u);
                            out += buf;
                        }
                        return out;
                    };
                    std::cerr << "[sif-rpc-hle] call sid=0x" << std::hex << sid << " fno=0x" << fno << std::dec
                              << " rmode=" << rmode << " send=" << sendSize << " recv=" << receiveSize
                              << " handled=" << handled;
                    if ((sid & 0x80000000u) == 0u || sid == 0x80000701u)
                        std::cerr << " sendData=" << preview(sendAddress, sendSize)
                                  << " recvData=" << preview(receive, receiveSize);
                    std::cerr << std::endl;
                }
            }"""
n = s.count(old)
if n != 1:
    print("FAIL matches", n)
    sys.exit(1)
open(P, "w").write(s.replace(old, new))
print("patched", P)
