#!/usr/bin/env python3
# Minimal, env-gated headless guard for the PRESENTATION layer only.
# Does NOT touch EE scheduler / generated code / kernel HLE behavior.
# Applied to the SCRATCH copy of ps2_runtime.cpp only.
import sys

p = "/tmp/ps2recomp_build/PS2Recomp/ps2xRuntime/src/lib/ps2_runtime.cpp"
s = open(p).read()
orig = s

def repl(old, new, s):
    n = s.count(old)
    if n != 1:
        print(f"FAIL: expected 1 match, found {n} for:\n{old[:80]}...")
        sys.exit(1)
    return s.replace(old, new)

# 0) ensure includes
anchor = '#include "ps2_runtime.h"'
if s.count(anchor) >= 1 and "// PS2X_HEADLESS_INCLUDES" not in s:
    s = s.replace(anchor, anchor + "\n// PS2X_HEADLESS_INCLUDES\n#include <cstdlib>\n#include <chrono>\n#include <thread>", 1)

# A) initialize(): gate InitWindow / audio / SetTargetFPS
old_a = """        SetConfigFlags(FLAG_WINDOW_RESIZABLE);
        InitWindow(HOST_WINDOW_WIDTH, HOST_WINDOW_HEIGHT, title);
        InitAudioDevice();
        m_audioBackend.setAudioReady(IsAudioDeviceReady());
#endif
        SetTargetFPS(60);"""
new_a = """        if (std::getenv("PS2X_HEADLESS") == nullptr) {
        SetConfigFlags(FLAG_WINDOW_RESIZABLE);
        InitWindow(HOST_WINDOW_WIDTH, HOST_WINDOW_HEIGHT, title);
        InitAudioDevice();
        m_audioBackend.setAudioReady(IsAudioDeviceReady());
        }
#endif
        if (std::getenv("PS2X_HEADLESS") == nullptr)
        SetTargetFPS(60);"""
s = repl(old_a, new_a, s)

# B1) run(): headless flag + guard framebuffer texture creation
old_b1 = """    // A blank image to use as a framebuffer
    Image blank = GenImageColor(FB_WIDTH, FB_HEIGHT, BLANK);
    Texture2D frameTex = LoadTextureFromImage(blank);
    UnloadImage(blank);"""
new_b1 = """    const bool ps2xHeadless = std::getenv("PS2X_HEADLESS") != nullptr;
    long ps2xHeadlessTimeoutSec = 120;
    if (const char* ps2xTo = std::getenv("PS2X_HEADLESS_TIMEOUT")) ps2xHeadlessTimeoutSec = std::atol(ps2xTo);
    const auto ps2xHeadlessStart = std::chrono::steady_clock::now();
    // A blank image to use as a framebuffer
    Texture2D frameTex{};
    if (!ps2xHeadless) {
    Image blank = GenImageColor(FB_WIDTH, FB_HEIGHT, BLANK);
    frameTex = LoadTextureFromImage(blank);
    UnloadImage(blank);
    }"""
s = repl(old_b1, new_b1, s)

# B2) loop body: in headless, skip all raylib present/draw via continue (+ wall-clock timeout)
old_b2 = """        uint32_t presentWidth = FB_WIDTH;
        uint32_t presentHeight = DEFAULT_DISPLAY_HEIGHT;
        UploadFrame(frameTex, this, presentWidth, presentHeight);"""
new_b2 = """        if (ps2xHeadless) {
            if (ps2xHeadlessTimeoutSec > 0 &&
                std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - ps2xHeadlessStart).count() >= ps2xHeadlessTimeoutSec) {
                RUNTIME_LOG("[headless] wall-clock timeout reached; requesting stop");
                requestStop();
                break;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(16));
            continue;
        }
        uint32_t presentWidth = FB_WIDTH;
        uint32_t presentHeight = DEFAULT_DISPLAY_HEIGHT;
        UploadFrame(frameTex, this, presentWidth, presentHeight);"""
s = repl(old_b2, new_b2, s)

# B3) cleanup: guard UnloadTexture/CloseWindow
old_b3 = """    UnloadTexture(frameTex);
    CloseWindow();"""
new_b3 = """    if (!ps2xHeadless) {
    UnloadTexture(frameTex);
    CloseWindow();
    }"""
s = repl(old_b3, new_b3, s)

open(p, "w").write(s)
print("PATCHED OK; bytes:", len(orig), "->", len(s))
