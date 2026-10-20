# way-of-the-samurai-decomp

Matching decompilation of **Way of the Samurai** (PS2, Acquire/Spike 2002; US SLUS-20407) — byte-exact C that recompiles to the original Emotion Engine (MIPS R5900) executable.

- Compiler: **ee-gcc 2.96** (confirmed by byte-exact matches).
- Pipeline: [decomp.wtf / decompnet](https://github.com/i12bp8/decomp.wtf) — splat + spimdisasm + m2c + compile-and-byte-compare oracle.
- `src/` — matched C sources + `context.h` (exported from decompnet).
- `tools/` — disc/ELF extraction helper.
- The disc image and extracted executable are **gitignored** — supply your own legally obtained copy.

Status: work in progress.
