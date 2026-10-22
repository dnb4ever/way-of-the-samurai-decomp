#!/usr/bin/env python3
# Scratch-tree-only patch 8: always answer RPC_CALL with RPC_END (the EE's sceSifCheckStatRpc polls the
# packet-busy bit that only the RPC_END handler clears, also for SIF_RPC_M_NOWAIT calls), plus a bounded
# per-call trace for the services map.
import sys

P = "/tmp/ps2recomp_build/PS2Recomp/ps2xRuntime/src/lib/Kernel/Stubs/SIF.cpp"
s = open(P).read()
old = """            // rmode == 0 means the caller asked for SIF_RPC_M_NOWAIT without an end function: the IOP
            // sends no RPC_END for those.
            if (rmode != 0u)
                deliverSifReply(rdram, makeRpcEndPacket(packet, kSifCmdRpcCall, serverHandle,
                                                        server ? server->buff : 0u, server ? server->cbuff : 0u));"""
new = """            {
                static uint32_t s_callLogs = 0u;
                if (s_callLogs < 96u)
                {
                    ++s_callLogs;
                    std::cerr << "[sif-rpc-hle] call sid=0x" << std::hex << sid << " fno=0x" << fno << std::dec
                              << " rmode=" << rmode << " send=" << sendSize << " recv=" << receiveSize
                              << " handled=" << handled << std::endl;
                }
            }
            // The IOP answers every call with RPC_END: that is what releases the EE-side packet, and
            // sceSifCheckStatRpc polls that busy bit for SIF_RPC_M_NOWAIT calls. rmode only tells the IOP
            // whether the caller is parked on a semaphore / wants an end function; the EE handler sorts it out.
            deliverSifReply(rdram, makeRpcEndPacket(packet, kSifCmdRpcCall, serverHandle,
                                                    server ? server->buff : 0u, server ? server->cbuff : 0u));"""
n = s.count(old)
if n != 1:
    print("FAIL: matches =", n)
    sys.exit(1)
open(P, "w").write(s.replace(old, new))
print("patched", P)
