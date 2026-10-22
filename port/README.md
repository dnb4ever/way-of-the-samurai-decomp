# WotS1 native-port runtime snapshot (2026-10-22)
Hand-edited ran-j/PS2Recomp (HEAD eb8688b) runtime that boots WotS1 through IOP module load into filesystem mount.
Generated game code (3124 fns) lives in ../recomp-splat/output (regenerable); this snapshot is the UNIQUE work:
edited ps2xRuntime/ + the *_patch.py scripts + logs/. .orig/.fixN backups are beside each edited file.

## Build
rsync a fresh PS2Recomp, copy recomp-splat/output/*.cpp -> ps2xRuntime/src/runner/ and the 2 headers -> include/,
overlay this ps2xRuntime/, then:
  cmake -S PS2Recomp -B build -DCMAKE_BUILD_TYPE=RelWithDebInfo -DPS2X_BUILD_RECOMP=OFF -DPS2X_BUILD_ANALYZER=OFF     -DPS2X_BUILD_TEST=OFF -DPS2X_BUILD_STUDIO=OFF -DPS2X_ENABLE_DEBUG_UI=OFF     -DCMAKE_CXX_FLAGS="-isystem <xdev>/usr/include -march=native"
  cmake --build build --target ps2EntryRunner -j32

## Run (needs extracted disc root + ISO)
disc_root = game.iso extracted (minus MOVIE/) + mc0/mc1 ; ELF = out/SLUS_204.07 (byte-identical)
  cd build/ps2xRuntime && PS2X_HEADLESS=1 PS2X_HEADLESS_TIMEOUT=60     PS2X_CD_IMAGE=/home/battlebeast/decomp/wots-ps2/out/game.iso ./ps2EntryRunner <disc_root>/SLUS_204.07

## State (2026-10-22)
Boots through: InitHeap, CdVdInit, IOP reboot (IOPRP243.IMG), SIF re-init, all 7 IOP modules loaded
(sio2man/padman/mcman/mcserv/libsd via HLE; sdr_driver/gzmfs_driver on the R3000 core), libsd+GZMFS RPC bound,
"Init File System Begin". STALL: emulated gzmfs_driver worker stuck "busy" (state 2) after cmd 0x0D - needs
ps2xIOP-internal tracing + libsd imports (sceSdInit 4/5/11/26). NOTE: PS2Recomp is GPL-3.0 (port inherits it).
