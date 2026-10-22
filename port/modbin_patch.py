#!/usr/bin/env python3
# Scratch-tree-only patch 7: LOADFILE fno 0 fallback for IRX files that only exist inside the game's
# MODULE.BIN container (this disc ships no loose IRX files). Retail reaches these modules through
# FILEIO(open/lseek/read MODULE.BIN) + sceSifLoadModuleBuffer; that path is not emulated yet, so the
# container is served directly and the shortcut is logged as such.
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


rep("""        // LOADFILE (rom0:LOADFILE, sid 0x80000006). fno 0 = LF_F_MOD_LOAD:""",
    """        // The game packs its IOP modules in cdrom0:\\MODULE.BIN: { u32 count; u32 0x40; u32 0x40; u32 pad;
        // u32 sizes[count]; ... } followed by the concatenated IRX ELFs starting at 0x40. Retail loads them with
        // FILEIO + sceSifLoadModuleBuffer; until that path is emulated, serve a requested X.IRX from the container.
        bool loadIrxFromModuleBin(uint8_t *rdram, PS2Runtime *runtime, const std::string &requestPath,
                                  const uint8_t *args, uint32_t argBytes, ps2x::iop::ModuleLoadResult &out)
        {
            std::string base = requestPath;
            const size_t slash = base.find_last_of("\\\\/");
            if (slash != std::string::npos)
                base = base.substr(slash + 1u);
            const size_t semi = base.find(';');
            if (semi != std::string::npos)
                base = base.substr(0, semi);
            const size_t dot = base.find('.');
            if (dot != std::string::npos)
                base = base.substr(0, dot);
            for (char &ch : base)
                ch = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
            if (base.empty())
                return false;

            const std::filesystem::path containerPath = PS2Runtime::getIoPaths().cdRoot / "MODULE.BIN";
            std::ifstream file(containerPath, std::ios::binary);
            if (!file)
                return false;
            std::vector<uint8_t> container((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
            if (container.size() < 0x40u)
                return false;
            const uint32_t count = readPacketU32(container.data(), 0u);
            if (count == 0u || count > 11u || 0x10u + count * 4u > 0x40u)
                return false;

            size_t offset = 0x40u;
            for (uint32_t i = 0; i < count; ++i)
            {
                const uint32_t size = readPacketU32(container.data(), 0x10u + i * 4u);
                if (size < 0x100u || offset + size > container.size())
                    return false;
                const uint8_t *blob = container.data() + offset;
                // Module name lives in the .iopmod header near the start of the image; collect printable runs.
                bool match = false;
                std::string run;
                const size_t scan = std::min<size_t>(size, 0x400u);
                for (size_t k = 0; k <= scan && !match; ++k)
                {
                    const char ch = k < scan ? static_cast<char>(blob[k]) : '\\0';
                    if (k < scan && (std::isalnum(static_cast<unsigned char>(ch)) || ch == '_'))
                    {
                        run.push_back(static_cast<char>(std::tolower(static_cast<unsigned char>(ch))));
                        continue;
                    }
                    if (run.size() >= 4u)
                    {
                        const std::string prefix = run.substr(0, run.find('_'));
                        match = run == base || run.rfind(base, 0) == 0 ||
                                (prefix.size() >= 3u && base.rfind(prefix, 0) == 0);
                    }
                    if (!match)
                        run.clear();
                }
                if (match)
                {
                    const uint32_t guest = runtime->guestMalloc(size, 16u);
                    if (guest == 0u || !writeEeRange(rdram, guest, blob, size))
                    {
                        if (guest != 0u)
                            runtime->guestFree(guest);
                        return false;
                    }
                    out = runtime->loadIopModuleBuffer(guest, args, argBytes);
                    runtime->guestFree(guest);
                    std::cerr << "[sif-rpc-hle] LOADFILE: '" << requestPath << "' is not a disc file; served module '" << run
                              << "' (" << size << " bytes) from MODULE.BIN entry " << i
                              << " [container shortcut: retail uses FILEIO + LoadModuleBuffer] -> handled="
                              << out.handled << " id=" << out.moduleId << " start=" << out.startResult << std::endl;
                    return true;
                }
                offset += size;
            }
            return false;
        }

        // LOADFILE (rom0:LOADFILE, sid 0x80000006). fno 0 = LF_F_MOD_LOAD:""")

rep("""            const ps2x::iop::ModuleLoadResult loaded = runtime->loadIopModule(path, args, argBytes);
            std::cerr << "[sif-rpc-hle] LOADFILE load '" << path << "' arglen=" << argLen""",
    """            ps2x::iop::ModuleLoadResult loaded = runtime->loadIopModule(path, args, argBytes);
            if ((!loaded.handled || loaded.moduleId < 0) && !loadIrxFromModuleBin(rdram, runtime, path, args, argBytes, loaded))
                std::cerr << "[sif-rpc-hle] LOADFILE: no disc file, HLE provider or MODULE.BIN entry for '" << path << "'" << std::endl;
            std::cerr << "[sif-rpc-hle] LOADFILE load '" << path << "' arglen=" << argLen""")

if "#include <fstream>" not in s:
    rep('#include <deque>\n', '#include <deque>\n#include <fstream>\n#include <iterator>\n')

open(P, "w").write(s)
print("patched", P)
