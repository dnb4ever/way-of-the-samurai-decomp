# Way of the Samurai (PS2) — Matching Decompilation + Native Port

A work-in-progress **matching decompilation** of *Way of the Samurai* (PS2, `SLUS-20407`, NTSC-U, 2002, Acquire/Bam) and a parallel **native PC port** via static recompilation.

> **Status: active WIP, greenfield** — no prior public decomp of this game existed.
> This is an **AI-assisted** project (an LLM paired with a byte-exact compile oracle). It is original
> work on a game nobody else was decompiling — not a fork or "finish" of anyone else's project.

## Progress
- **Matching decomp:** progress is computed by every build, never kept by hand. `tools/build.sh` prints two
  numbers (also in `build/summary.txt`, and `report.json` for objdiff/ByteMatch):
  - **matched under the rules**: byte-exact *and* free of fakematch patterns (the headline number);
  - **bytes matched**: byte-exact, including functions still `MATCH-PENDING`.
  Total functions in the game: ~3143.
- **Native port:** the recompiled Emotion Engine code **runs natively** and boots through kernel init
  → IOP reboot + all 7 IOP modules → filesystem mount → the game's main loop, emitting its first
  Graphics Synthesizer primitives. Rendering (GS) and VU1 are the long pole and not yet implemented.

## Build / verify
Requires Linux or WSL (Ubuntu) with `gcc`, `curl`, `rsync` and `binutils-mips-linux-gnu`.

1. Put your own dump's executable at `disc/SLUS_204.07` (SHA-1 in `config/SLUS_204.07.sha1`). From a
   Redump `.bin`: `cc -O2 -o extract_elf tools/extract_elf.c && ./extract_elf "Way of the Samurai (USA).bin" disc/SLUS_204.07`.
   **No game data is included** (ISO/ELF/BIN are gitignored).
2. Run `tools/build.sh`. It fetches the stock compilers (pinned by hash), builds every unit in
   `config/units.txt`, checks every function against retail, and runs the **byte gate**: all functions are
   written over a copy of the retail executable, whose SHA-1 must stay unchanged. It ends with `GATE OK`.

- **Compilers:** the unmodified Sony builds distributed by decomp.me — `ee-gcc 2.9-ee-991111` (SDK
  libraries) and `ee-gcc 2.96-ee-001003-1` (newlib, `-G0`), `-O2`, `-G8` for game code. Each unit records the
  one that reproduces it. No compiler is patched: a modified compiler that imitates retail output proves
  nothing about the source. Game functions that save callee-saved registers use a prologue none of the
  public builds emit; they are parked (`#ifdef NON_MATCHING`) until the right compiler build is found. See
  `AGENTS.md`, "Toolchain".
- **Rules:** source follows the decomp code rules in `AGENTS.md`; `tools/rules.c` checks them.
- **Port:** static recompilation over [ran-j/PS2Recomp](https://github.com/ran-j/PS2Recomp) (GPL-3.0);
  our runtime work is the set of patches in `port/` applied over PS2Recomp @ `eb8688b`.

## Contributing
Open, in the spirit of **share-and-share-alike**. Fixes, matches, and especially help on the
**GS / VU1 graphics path** are very welcome. Progress tracked via objdiff.

## License
Decomp sources/config: see `LICENSE`. The port runtime, derived from PS2Recomp, is **GPL-3.0**.
Game assets/executable are not distributed and are not covered.
