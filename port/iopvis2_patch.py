#!/usr/bin/env python3
# Scratch-tree-only: PS2X_CD_IMAGE env hook + IOP host file traces (position based).
R = "/tmp/ps2recomp_build/PS2Recomp/ps2xRuntime"

p = R + "/src/lib/ps2_runtime.cpp"
s = open(p).read()
if "PS2X_CD_IMAGE" not in s:
    i = s.index("void PS2Runtime::configureIoPathsFromElf")
    j = s.index("    setIoPaths(paths);\n}", i)
    hook = """    // Scratch hook: nothing in the runtime ever populates cdImage, yet sector-level cdvd reads
    // (IOP cdvdman HLE, CD stubs) need the ISO. PS2X_CD_IMAGE=<path> selects it explicitly.
    if (const char *cdImageEnv = std::getenv("PS2X_CD_IMAGE"))
    {
        if (cdImageEnv[0] != 0)
        {
            paths.cdImage = std::filesystem::path(cdImageEnv);
            std::cerr << "[io-paths] cdImage=" << paths.cdImage.string() << " (PS2X_CD_IMAGE)" << std::endl;
        }
    }
"""
    s = s[:j] + hook + s[j:]
    open(p, "w").write(s)
    print("env hook inserted")

p = R + "/src/lib/ps2_iop_host.cpp"
s = open(p).read()
if "[iop-host] openHostFile" not in s:
    a = s.index("PS2IopHostAdapter::openHostFile(")
    b = s.index("    if (!stream)", a)
    s = s[:b] + """    {
        static uint32_t s_openLogs = 0u;
        if (s_openLogs < 64u)
        {
            ++s_openLogs;
            std::cerr << "[iop-host] openHostFile " << hostPath.string() << " -> " << (stream ? "ok" : "FAILED") << std::endl;
        }
    }
""" + s[b:]
    a = s.index("bool PS2IopHostAdapter::readHostFile(uint64_t handle,")
    b = s.index("{", a)
    s = s[:b] + """{
    {
        static uint32_t s_readLogs = 0u;
        if (s_readLogs < 48u)
        {
            ++s_readLogs;
            std::cerr << "[iop-host] readHostFile handle=" << handle << " offset=" << offset << " size=" << size << std::endl;
        }
    }""" + s[b + 1:]
    open(p, "w").write(s)
    print("host traces inserted")
