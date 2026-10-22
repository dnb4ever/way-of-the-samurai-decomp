#!/usr/bin/env python3
# Scratch-tree-only patch: make Deci2Call(ReqSend) drive the guest DECI2 handler
# (DECI2_WRITE -> DECI2_WRITEDONE) like the EE kernel's DECI2 manager does, and
# enable the Deci2Call HLE (it was behind a never-defined RUNTIME_DECI2CALL).
import sys

ROOT = "/tmp/ps2recomp_build/PS2Recomp/ps2xRuntime"


def patch(path, pairs):
    s = open(path).read()
    for old, new in pairs:
        n = s.count(old)
        if n != 1:
            print(f"FAIL {path}: expected 1 match, found {n} for:\n{old[:120]}")
            sys.exit(1)
        s = s.replace(old, new)
    open(path, "w").write(s)
    print(f"patched {path}")


# ---------------------------------------------------------------- Deci2.cpp
deci2 = ROOT + "/src/lib/Kernel/Syscalls/Deci2.cpp"

old_close = """    static bool closeDeci2Socket(int32_t socket)
    {
        std::lock_guard<std::mutex> lock(g_deci2Mutex);
        return g_deci2Sessions.erase(socket) != 0;
    }
}
"""
new_close = """    static bool closeDeci2Socket(int32_t socket)
    {
        std::lock_guard<std::mutex> lock(g_deci2Mutex);
        return g_deci2Sessions.erase(socket) != 0;
    }

    static bool findDeci2Session(int32_t socket, Deci2Session &out)
    {
        std::lock_guard<std::mutex> lock(g_deci2Mutex);
        auto sessionIt = g_deci2Sessions.find(socket);
        if (sessionIt == g_deci2Sessions.end())
        {
            return false;
        }
        out = sessionIt->second;
        return true;
    }

    // DECI2 handler events (sceDeci2Open handler(int event, int param, void *opt)).
    constexpr uint32_t kDeci2EventWrite = 3u;     // DECI2_WRITE: handler pushes its packet via sceDeci2ExSend
    constexpr uint32_t kDeci2EventWriteDone = 4u; // DECI2_WRITEDONE: handler marks the request complete
    constexpr uint16_t kDeci2ProtocolDttyEe = 0x0210u;
    constexpr uint32_t kDeci2DttyHeaderBytes = 12u; // 8-byte DECI2 header + 4-byte DTTY header
}
"""

old_send = """        // WE dont need to do thouses
        case 3:  // sceDeci2ReqSend(socket, dest)
        case 4:  // sceDeci2Poll(socket)
        case -7: // sceDeci2ExReqSend(socket, dest)
        {
            setReturnS32(ctx, KE_OK);
            return;
        }
"""
new_send = """        case 3: // sceDeci2ReqSend(socket, dest)
        {
            // The EE kernel's DECI2 manager services a send request by invoking the socket's
            // handler with DECI2_WRITE (the handler transmits via sceDeci2ExSend) and then
            // DECI2_WRITEDONE (the handler clears its own in-flight flag, e.g. libkernel's
            // DTTY writer spins on sceDeci2Poll until that flag drops). No host debugger is
            // attached, so the request completes immediately: run both events on the calling
            // thread's invocation stack, then resume after the syscall with v0 = 0.
            const int32_t socket = static_cast<int32_t>(args[0]);
            Deci2Session session;
            if (!findDeci2Session(socket, session) || session.handler == 0u ||
                !runtime->hasFunction(session.handler))
            {
                setReturnS32(ctx, KE_ERROR);
                return;
            }

            EeScheduler &scheduler = runtime->eeScheduler();
            scheduler.bindMainContextForSyscall(*ctx, rdram);

            std::vector<GuestInvocation> invocations;
            for (uint32_t event : {kDeci2EventWrite, kDeci2EventWriteDone})
            {
                GuestInvocation invocation{};
                invocation.kind = GuestInvocationKind::HleCall;
                invocation.tag = (0x7Cu << 8) | event;
                invocation.context = *ctx;
                invocation.context.pc = session.handler;
                SET_GPR_U32(&invocation.context, 4, event);
                SET_GPR_U32(&invocation.context, 5, 0u); // param: unused for write events
                SET_GPR_U32(&invocation.context, 6, session.opt);
                SET_GPR_U32(&invocation.context, 29, 0u); // invokeCurrentSequence assigns the invocation stack
                SET_GPR_U32(&invocation.context, 31, 0u);
                invocations.push_back(std::move(invocation));
            }
            invocations.back().onComplete = [](const R5900Context &, R5900Context &parent)
            {
                setReturnS32(&parent, KE_OK);
            };
            scheduler.invokeCurrentSequence(std::move(invocations));
        }
        case 4:  // sceDeci2Poll(socket): nothing pending, the request already completed in ReqSend
        case -7: // sceDeci2ExReqSend(socket, dest)
        {
            setReturnS32(ctx, KE_OK);
            return;
        }
"""

old_exsend = """            if (bufferAddr != 0u && length != 0u)
            {
                const uint32_t clampedLength = std::min<uint32_t>(length, 512u);
                std::string text;
                text.reserve(clampedLength);
                for (uint32_t byteIndex = 0; byteIndex < clampedLength; byteIndex++)
                {
                    text.push_back(static_cast<char>(rdram[(bufferAddr + byteIndex) & PS2_RAM_MASK]));
                }
                logDeci2Text("[Deci2Call:send] ", text);
            }
"""
new_exsend = """            if (bufferAddr != 0u && length != 0u)
            {
                // DTTY packets carry a DECI2 + DTTY header before the text; skip it for the log.
                Deci2Session session;
                uint32_t skip = 0u;
                if (findDeci2Session(static_cast<int32_t>(args[0]), session) &&
                    session.protocol == kDeci2ProtocolDttyEe && length > kDeci2DttyHeaderBytes)
                {
                    skip = kDeci2DttyHeaderBytes;
                }
                const uint32_t clampedLength = std::min<uint32_t>(length, 512u);
                std::string text;
                text.reserve(clampedLength);
                for (uint32_t byteIndex = skip; byteIndex < clampedLength; byteIndex++)
                {
                    text.push_back(static_cast<char>(rdram[(bufferAddr + byteIndex) & PS2_RAM_MASK]));
                }
                logDeci2Text(skip ? "[Deci2:tty] " : "[Deci2Call:send] ", text);
            }
"""

patch(deci2, [(old_close, new_close), (old_send, new_send), (old_exsend, new_exsend)])

# ------------------------------------------------------------ CMakeLists.txt
cmake = ROOT + "/CMakeLists.txt"
old_opt = """option(PS2X_STRICT_RETURN_DIAGNOSTICS "Route generated JR $ra returns through runtime branch diagnostics" OFF)
"""
new_opt = """option(PS2X_STRICT_RETURN_DIAGNOSTICS "Route generated JR $ra returns through runtime branch diagnostics" OFF)
option(PS2X_ENABLE_DECI2CALL "Enable the Deci2Call (syscall 0x7C) HLE used by libkernel TTY/printf" ON)
"""
old_def = """if(PS2X_STRICT_RETURN_DIAGNOSTICS)
    target_compile_definitions(ps2_runtime PUBLIC
        PS2X_STRICT_RETURN_DIAGNOSTICS=1
    )
endif()
"""
new_def = """if(PS2X_STRICT_RETURN_DIAGNOSTICS)
    target_compile_definitions(ps2_runtime PUBLIC
        PS2X_STRICT_RETURN_DIAGNOSTICS=1
    )
endif()

if(PS2X_ENABLE_DECI2CALL)
    target_compile_definitions(ps2_runtime PRIVATE
        RUNTIME_DECI2CALL=1
    )
endif()
"""
patch(cmake, [(old_opt, new_opt), (old_def, new_def)])
print("OK")
