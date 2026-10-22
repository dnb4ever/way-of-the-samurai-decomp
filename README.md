# Way of the Samurai (PS2) — Matching Decompilation + Native Port

A work-in-progress **matching decompilation** of *Way of the Samurai* (PS2, `SLUS-20407`, NTSC-U, 2002, Acquire/Bam) and a parallel **native PC port** via static recompilation.

> **Status: active WIP, greenfield** — no prior public decomp of this game existed.
> This is an **AI-assisted** project (an LLM paired with a byte-exact compile oracle). It is original
> work on a game nobody else was decompiling — not a fork or "finish" of anyone else's project.

## Progress
- **Matching decomp:** **382** functions committed as byte-exact matches (see `src/src/`). Of these,
  **~266 reproduce byte-identical under the current unified compiler**; the rest are earlier matches
  pending re-verification against the current compiler — tracked honestly in **`MATCHES.tsv`**
  (`yes` / `no` / `legacy`). Total functions in the game: ~3143.
- **Native port:** the recompiled Emotion Engine code **runs natively** and boots through kernel init
  → IOP reboot + all 7 IOP modules → filesystem mount → the game's main loop, emitting its first
  Graphics Synthesizer primitives. Rendering (GS) and VU1 are the long pole and not yet implemented.

## Build / verify
- **Target:** `SLUS_204.07`. **No game data is included** (ISO/ELF/BIN are gitignored) — bring your own dump.
- **Compiler:** Sony **ee-gcc 2.9 (991111 line)**, source-patched to match this game's codegen
  (128-bit `sq`/`sd` callee-saves; `$at`-register address splits for symbolic/absolute/large-offset
  memory accesses), built `-O2 -G8`.
- **Verify:** each match is byte-exact against the retail function (compile → link at the real address
  → byte-compare). Score 0 or it doesn't count.
- **Port:** static recompilation over [ran-j/PS2Recomp](https://github.com/ran-j/PS2Recomp) (GPL-3.0);
  our runtime work is the set of patches in `port/` applied over PS2Recomp @ `eb8688b`.

## Contributing
Open, in the spirit of **share-and-share-alike**. Fixes, matches, and especially help on the
**GS / VU1 graphics path** are very welcome. Progress tracked via objdiff.

## License
Decomp sources/config: see `LICENSE`. The port runtime, derived from PS2Recomp, is **GPL-3.0**.
Game assets/executable are not distributed and are not covered.
