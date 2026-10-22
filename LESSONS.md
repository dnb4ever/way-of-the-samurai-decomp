# Matching Decomp — Lessons Learned

*A field guide distilled from a real matching decompilation of Way of the Samurai 1 (PS2, ee-gcc). The specifics are PS2-flavored, but the method transfers to any matching decomp.*

## 1. A "floor" is usually a compiler-ID problem, not a ceiling

The single biggest lesson. When a large set of functions seems unmatchable, the instinct is to call them compiler floors (impossible from any C). Often they are not floors at all — they are evidence your compiler, or its configuration, is wrong or incomplete.

- **Tell-tale sign: many functions fail the same mechanical way.** Here, ~1,585 functions (half the game) differed from our compiler by one thing — the ROM saved callee-saved registers with 128-bit `sq`/`lq`, while our ee-gcc 2.96 emitted 64-bit `sd`/`ld`. A systematic, identical difference across hundreds of functions is a compiler signature, not hundreds of coincidental floors.
- **Compiler-ID can be a false positive.** We "confirmed" ee-gcc 2.96 because it matched the simple, spill-free functions. It was incomplete: the game was actually built with a different compiler (Cygnus 2.9-ee-991111b) whose codegen spills callee registers with `sq`. The functions that matched under both compilers hid the discrepancy.
- **Action:** before writing off a floor class, ask whether it is *one mechanical difference repeated*. If yes, suspect the compiler before you suspect C.

## 2. Diagnose structurally; let the byte-oracle referee everything

- **Detect floor patterns from the binary, not from the LLM's say-so.** A structural detector (does a word decode to `sq $sN, off($sp)` for a callee reg?) found the pattern with zero false positives — validated against ground truth: *0 of 314 already-matched functions* carried the signature. Structural detection beats trusting agent notes, which can be confidently wrong.
- **The oracle is everything.** Every claim — this patch helps, this is a floor, this compiler is right — is settled by the byte-compare score (0 = match). Never trust reasoning over the oracle.
- **Validate on an *unbiased* sample.** We nearly discarded a good patch because we tested it on functions selected by stale notes (biased toward the hard cases). A random-sample A/B (mean score, deployed vs patched) gave the true answer. Note-based samples lie; random samples do not.

## 3. Building a period compiler on a modern host is doable — userspace, no root

We built a patched 1999 ee-gcc from source on modern Ubuntu, entirely in userspace. The walls you hit, in order:

- **Modern gcc rejects 1999 C.** gcc 14+ makes K&R implicit-int and empty `()` hard errors. Fix with `-std=gnu89 -fpermissive -Wno-implicit-int -Wno-implicit-function-declaration -Wno-int-conversion -Wno-return-mismatch -Wno-incompatible-pointer-types`.
- **The grammar needs period bison.** Old gcc's `c-parse.y` uses mid-rule actions modern bison (3.x) rejects. Build **bison 1.28** from source (`--disable-nls`, same legacy flags) and point the gcc build at it via `PATH` + `BISON_SIMPLE`/`BISON_HAIRY`.
- **32-bit build without root.** The compiler builds 32-bit for codegen fidelity. No multilib and no root? `apt-get download libc6-dev-i386 lib32gcc-*-dev` (download needs no root), `dpkg -x` into a userspace sysroot, then point gcc at it with `-B`, `-idirafter`, `-L`, and `--dynamic-linker=<sysroot>/ld-linux.so.2`. Rewrite the `libc.so` linker-script's absolute paths into the sysroot.
- **Prior art saves days.** The hard knowledge — which source tarball, the patch mechanism, the bison trick — came from other PS2 decomp projects. A build that "would take days" took about an hour because the recipe already existed. Search the scene before reinventing.

## 4. You can author compiler patches that don't exist

- **The method is tight iteration, not magic:** read the gcc internal responsible, form a hypothesis, compile a test, read the exact diff/error, adjust, repeat — always against the oracle. We authored three patches for our exact case: a tuned quad-saves (s-regs via `sq`, `ra` via `sd`), per-register mode-aware frame-offset math in `mips.c`, and a typedef-redefinition leniency in `c-decl.c`.
- **Patches from another game don't always transfer.** The sq/save-order fixes were compiler-ID-level (universal); finer class patches (scheduling, tuned for a different title) gave no net gain here and one regressed. Validate each patch against *your* binary, not the source project's.
- **Watch coupled internals.** Our save-order bug was subtle: the frame math used a single uniform slot size, incompatible with a mixed layout (16-byte s-regs, 8-byte `ra`). The fix had to make offset computation per-register-mode-aware — reading the whole prologue-emission path, not just the obvious knob.

## 5. Deploying to a live project — and the conversion gap

- **Deploy without disruption.** The harness read each project's compiler config fresh from the DB and accepted a direct command, so no restart was needed. We re-baselined by editing one config row — verify-first (compile already-matched functions; commit only if they stay score 0), fully reversible.
- **A capable compiler is not matches — this is the humbling one.** Unlocking half the game at the *compiler* level did not produce matches. The agents still had to write byte-perfect C. "Matchable in principle" and "matched in practice" are different phases; the second is ordinary decomp grind, and the compiler only removes the wall in front of it.
- **Stale floor notes poison reopened work.** After the compiler change, old "PERMANENTLY UNMATCHABLE" notes made agents re-reject the now-matchable functions (and burn tokens re-confirming dead floors). After any compiler change, purge the obsolete floor notes and signal the change (we banner-ed the shared header). Don't let analysis outlive the compiler it was written for.
- **Compile-fails are often C quality, not scaffolding.** Our shared header declared ~107 of 3,143 symbols; the rest are declared per-function by the agent. A *global* externs fix conflicted with that model and broke matches; a per-compile auto-extern wrapper fixed only ~15% (most fails were missing struct types and type errors). There was no silver bullet — it is real per-function work.

## 6. Operational pitfalls that cost real time

- **`pkill -f <pattern>` self-matches** the shell running it when the pattern is in your own command line, so it kills itself (exit 255). Run the kill from a script file, or kill by PID with a pattern unique to the targets.
- **Automated floor-exclusion must be read-only or it fights you.** A sweep that auto-excluded kept re-flooring the functions we had just reopened (structural detector plus stale notes). A monitor that *flags* is safe; one that *acts* needs a quality/recency gate or it quietly undoes your progress.
- **Local-LLM throughput scales with *active* params and interconnect, not total size.** A 3B-active MoE on one GPU beat a 10B-active 122B split across two (no cross-GPU sync tax). Match agent count to serving slots — oversubscription just queues (dead time invisible in tok/s). Local-model "token burn" is free compute; the real cost is frontier agents plus the driving session.
- **Verify-first, always.** Every destructive step compiled a sample of known-good functions and aborted on any regression. Nothing broke across the entire effort because of that one discipline.

---

*Bottom line: in matching decomp the compiler is the lever and the byte-oracle is the referee. Treat "floors" as hypotheses about the compiler, detect them structurally, author the fix, and verify everything against score 0. Getting the compiler right is the breakthrough; converting the unlocked functions into actual matches is a separate, ordinary grind — don't mistake one for the other.*
