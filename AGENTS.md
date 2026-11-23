# Way of the Samurai (SLUS-204.07): agent and contributor guide

Reconstruct source that the original compilers turn into the retail code byte for byte, function by function,
and that a reader can understand. Correct structure outranks any score. Every function must pass the reader
test of the decomp code rules: would a programmer on the original team have typed this? Source that matches
through tricks (a fakematch) is not a match.

This file takes precedence over generic guidance. The code rules are the `decomp-code-rules` catalogue
(rule numbers R1-R37 below refer to it); this file's **Project profile** overrides it where they differ.

## Project adapter

| verb | this project's command |
|---|---|
| `build` | `tools/build.sh`: clean rebuild of every unit in `config/units.txt`, per-function check, byte gate, rule check |
| `match <unit>` | `tools/build.sh src/path/unit.c` (compiles and checks just that unit) |
| `diff <function>` | `tools/diff.sh <function>` after a build of its unit (`tools/build.sh -n ...` for a parked body) |
| `report` | written by `build`: `build/summary.txt` (both headline numbers), `build/report.json` (objdiff measures) |
| `gate` | `tools/build.sh` must exit 0 and print `GATE OK`; `build/rules.txt` must not gain [match] findings |
| `disasm` / `xref` | `mips-linux-gnu-objdump -D -b binary -m mips:5900 -EL --adjust-vma=0x100000` over the segment; callers by `grep` for `jal 0x...` |
| `rules [<files>]` | `rules` (built from `tools/rules.c`; `build` runs it over every unit and header) |
| `permute` | none |

Everything runs on Linux or WSL. `tools/build.sh` fetches the compilers (pinned by hash) into
`~/.cache/wots/compilers`, builds the C tools, and mirrors the tree to a Linux-native directory when the checkout
is on a Windows drive (the 32-bit compilers cannot `stat` files under `/mnt`).

### The byte gate

`tools/fncheck` relocates every function of every built object at its retail address and compares it with the
retail bytes. References into an object's own data (literal pools, strings, statics) are located through the
retail instruction and their contents must equal the retail bytes there. With `-H`, it writes every function over
a copy of the retail image and requires the image's SHA-1 to equal `config/SLUS_204.07.sha1`: a single wrong
byte anywhere fails the gate. "Matched" is claimed only from its printed `GATE OK` line.

Unfinished functions are parked in `#ifdef NON_MATCHING` and are not compiled into the gated build; a unit whose
functions are all parked does not appear in `config/units.txt` until one matches. `tools/build.sh -n` compiles
the parked bodies too and lists how far each is from retail, without gating them.

### Toolchain

The executable was linked by the Metrowerks linker (`.comment`: `MW MIPS C Compiler (2.4.1.01)`), but its code
comes from several compilers. Each unit records the compiler that reproduces it in `config/units.txt`:

| code | compiler (stock, from decomp.me) | flags | evidence |
|---|---|---|---|
| newlib (libc, libm) | `ee-gcc2.96` (2.96-ee-001003-1) | `-O2 -G0` | unmodified newlib source matches (`sprintf`) |
| Sony SDK libraries | `ee-gcc2.9-991111`, some libraries `ee-gcc2.96` | `-O2 -G8` | per library, by the functions they reproduce |
| game code | `ee-gcc2.9-991111` or `ee-gcc2.96` | `-O2 -G8` | leaf functions match both |

**Known gap:** game functions that save callee-saved registers use a prologue no published EE compiler emits
(`sq` for `$s` registers, `sd` for `$ra`, 16-byte slots, saved highest slot first). Neither 2.9-991111, 2.96,
the SN Systems 2.95 builds nor MWCC PS2 2.4 produces it. Those functions cannot be verified until the right
compiler build is found; they stay parked. Never patch or wrap a compiler to imitate retail output: a
modified compiler that reproduces the bytes proves nothing about the source.

`$gp` is `0x0022C0F0` (from `.reginfo`). Small data is reached through it in `-G8` units.

### Reference sources and how far each is trusted

- **newlib** (public; the Sony toolchain shipped it unmodified): adopt whole files verbatim, with their
  original comments and license headers. Presumed authentic when they match.
- **Function names from other games' symbols** (`config/symbols_debug.txt`): byte-identical library code in
  symbol-bearing PS2 builds. Kept only for library families and for names that 20+ unrelated games agree on.
  `config/symbol_candidates.txt` lists names that were *not* adopted (game code from unrelated titles, short
  shapes found everywhere); never apply them.
- **Sony SDK headers and sources** are not public. SDK functions keep their recorded names; their types are
  reconstructed from use and declared once in `include/sdk/`.

## Project profile

| item | this project |
|---|---|
| dialect (R22) | GNU C as accepted by GCC 2.9x: C89 plus GNU extensions (`__attribute__`, `long long`, mode(TI) 128-bit ints). Declarations at the top of a block. `//` comments are accepted. No C99 headers. Redefining a typedef is an error. |
| naming, newlib | newlib's own names and style, verbatim |
| naming, SDK | Sony's recorded names (`sceGifPkInit`, `_sysbitGet`); new names follow `sceLibVerbNoun` only with evidence; otherwise placeholders |
| naming, game code | no evidence of the original style yet: Table 1 (C engine) of the style defaults. Placeholders are address-derived: `func_XXXXXXXX`, `D_XXXXXXXX`; struct members `unk_XX` by offset |
| layout | K&R braces (a function's opening brace on its own line), 4-space indent, braces on every block, 79-column soft limit; unit opener `/* unit.c -- purpose */`; function banner (R33) as in `src/sdk/memclr.c`: `/*`, a `====` line, the name, an optional description, a `====` line, `*/`. newlib files keep newlib's layout |
| placeholder patterns (R7) | `func_`, `D_`, `unk_`, `var_`, `arg\d`, `temp_`, `sp[0-9A-F]+`, `M2C_`, any 6-8 digit hex run |
| evidence sources for names | `symbols_debug.txt`, newlib source, strings in the binary, call graph |
| line information | none: the executable is stripped and has no debug sections, so layout is [free] |
| parking (exempt from R37) | `#ifdef NON_MATCHING` around a function body that is not byte-exact |
| units | one source file per original object where the object is known (newlib files, SDK objects); game code whose object boundaries are unknown keeps one function per file under `src/game/` until evidence groups them |

### Rules this project relaxes or tightens

None yet. Rule changes are the maintainers' ruling, recorded here with the reason, with the checker updated in
the same commit.

## Ledgers

- **Matched:** computed by every build: `build/summary.txt` gives *matched under the rules* (the headline) and
  *bytes matched*. Never keep a hand-maintained match list.
- **MATCH-PENDING:** bytes exact but a [match] rule fails. The build lists them; they are open work.
- **RULE-EXCEPTION:** a style rule the bytes defeat, written at the code as
  `// RULE-EXCEPTION: <rule>, <reason the bytes give>`.
- **Parked:** `#ifdef NON_MATCHING`, listed by `tools/build.sh -n`.

## Tooling policy

Write project tools in **C**, not Python. Shell is acceptable only as glue around the C tools and the compilers.
Tools live in `tools/`, take their inputs from `config/`, and write only to `build/`.

## Repository hygiene

- Inputs in `config/`, generated output in the ignored `build/`, the retail executable in the ignored `disc/`.
- Docs describe current usage only: no diaries, scores, dates, agent names or campaign notes, in docs or in
  source comments (R21).
- Formatting and naming commits are separate from code changes, and are gated like any other.
- Commit messages name the change: `match: memclr`, `libc: adopt newlib sprintf.c`, `tools: ...`.
