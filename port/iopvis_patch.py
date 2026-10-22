#!/usr/bin/env python3
# Scratch-tree-only: IOP visibility (snapshot in tick log, host file traces) + PS2X_CD_IMAGE env hook.
import sys

R = "/tmp/ps2recomp_build/PS2Recomp/ps2xRuntime"


def patch(path, old, new):
    s = open(path).read()
    n = s.count(old)
    if n != 1:
        print(f"FAIL {path}: expected 1 match, found {n} for:\n{old[:160]}")
        sys.exit(1)
    open(path, "w").write(s.replace(old, new))
    print("patched", path)


# 1) tick emitter: IOP snapshot
patch(R + "/src/lib/ps2_runtime.cpp",
      """                for (const auto &thread : eeSnapshot.threads)
                {
                    RUNTIME_LOG("[run:thread] id=" << thread.id""",
      """                {
                    const ps2x::iop::DebugSnapshot iop = debugIopSnapshot();
                    RUNTIME_LOG("[run:iop] cycles=" << iop.emulatorCycles
                                                << " instr=" << iop.emulatorInstructions
                                                << " modules=" << iop.emulatorLoadedModules
                                                << " threads=" << iop.emulatorThreads
                                                << " rpcServers=" << iop.emulatorRpcServers
                                                << " diagnostics=" << iop.diagnostics.size()
                                                << std::endl);
                    for (const std::string &diagnostic : iop.diagnostics)
                    {
                        RUNTIME_LOG("[run:iop-diag] " << diagnostic << std::endl);
                    }
                }
                for (const auto &thread : eeSnapshot.threads)
                {
                    RUNTIME_LOG("[run:thread] id=" << thread.id""")

# 2) PS2X_CD_IMAGE env hook
patch(R + "/src/lib/ps2_runtime.cpp",
      """    if (!paths.elfDirectory.empty())
    {
        paths.hostRoot = paths.elfDirectory;
        paths.cdRoot = paths.elfDirectory;
        paths.mcRoot = paths.elfDirectory / "mc0";
    }
    setIoPaths(paths);
}""",
      """    if (!paths.elfDirectory.empty())
    {
        paths.hostRoot = paths.elfDirectory;
        paths.cdRoot = paths.elfDirectory;
        paths.mcRoot = paths.elfDirectory / "mc0";
    }
    // Scratch hook: nothing in the runtime ever populates cdImage, yet sector-level cdvd reads
    // (IOP cdvdman HLE, CD stubs) need the ISO. PS2X_CD_IMAGE=<path> selects it explicitly.
    if (const char *cdImageEnv = std::getenv("PS2X_CD_IMAGE"))
    {
        if (cdImageEnv[0] != '\\0')
        {
            paths.cdImage = std::filesystem::path(cdImageEnv);
            std::cerr << "[io-paths] cdImage=" << paths.cdImage.string() << " (PS2X_CD_IMAGE)" << std::endl;
        }
    }
    setIoPaths(paths);
}""")

# 3) host file traces
patch(R + "/src/lib/ps2_iop_host.cpp",
      """    const std::filesystem::path hostPath{std::string(path)};
#if defined(_WIN32)
    std::FILE *stream = ::_wfopen(hostPath.c_str(), L"rb");
#else
    std::FILE *stream = std::fopen(hostPath.string().c_str(), "rb");
#endif
    if (!stream)
    {
        return 0u;
    }""",
      """    const std::filesystem::path hostPath{std::string(path)};
#if defined(_WIN32)
    std::FILE *stream = ::_wfopen(hostPath.c_str(), L"rb");
#else
    std::FILE *stream = std::fopen(hostPath.string().c_str(), "rb");
#endif
    {
        static uint32_t s_openLogs = 0u;
        if (s_openLogs < 64u)
        {
            ++s_openLogs;
            std::cerr << "[iop-host] openHostFile '" << hostPath.string() << "' -> " << (stream ? "ok" : "FAILED") << std::endl;
        }
    }
    if (!stream)
    {
        return 0u;
    }""")

s = open(R + "/src/lib/ps2_iop_host.cpp").read()
anchor = "bool PS2IopHostAdapter::readHostFile(uint64_t handle,"
if s.count(anchor) != 1:
    print("FAIL readHostFile anchor")
    sys.exit(1)
# insert a trace at the top of readHostFile body: find the opening brace after the signature
idx = s.index(anchor)
brace = s.index("{", idx)
trace = """{
    {
        static uint32_t s_readLogs = 0u;
        if (s_readLogs < 48u)
        {
            ++s_readLogs;
            std::cerr << "[iop-host] readHostFile handle=" << handle << " offset=" << offset << " size=" << size << std::endl;
        }
    }"""
s = s[:brace] + trace + s[brace + 1:]
open(R + "/src/lib/ps2_iop_host.cpp", "w").write(s)
print("readHostFile trace inserted")
