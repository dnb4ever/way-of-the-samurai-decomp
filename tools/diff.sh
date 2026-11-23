#!/bin/bash
# diff.sh <function> -- retail vs compiled disassembly of one function, side by side.
#
# Run tools/build.sh (or tools/build.sh -n for a parked NON_MATCHING body) for
# the function's unit first; this reads the objects that build left behind.
set -u
REPO=$(cd "$(dirname "$0")/.." && pwd)
NAME=$1
case "$REPO" in
/mnt/*) WORK=${WOTS_MIRROR:-$HOME/.cache/wots/mirror/$(echo "$REPO" | tr '/' '_')} ;;
*)      WORK=$REPO ;;
esac

ADDR=$(grep -h "^$NAME = " "$REPO"/config/symbols.txt "$REPO"/config/symbols_debug.txt | head -1 | sed 's/.*= \(0x[0-9a-fA-F]*\);.*/\1/')
if [ -z "$ADDR" ]; then
    ADDR=0x$(echo "$NAME" | sed -n 's/.*_\([0-9A-Fa-f]\{6,8\}\)$/\1/p')
fi
[ "$ADDR" != "0x" ] || { echo "no address for $NAME"; exit 2; }

OBJ=""
for dir in "$WORK/build/obj-nonmatching" "$WORK/build/obj"; do
    [ -d "$dir" ] || continue
    OBJ=$(find "$dir" -name '*.o' -exec sh -c 'mips-linux-gnu-nm "$1" 2>/dev/null | grep -q " [Tt] '"$NAME"'$" && echo "$1"' _ {} \; | head -1)
    [ -n "$OBJ" ] && break
done
[ -n "$OBJ" ] || { echo "$NAME is not in any built object"; exit 2; }

SIZE=$(mips-linux-gnu-nm -S "$OBJ" | awk -v n="$NAME" '$4==n{print strtonum("0x"$2)}')
TEXT=$(mktemp)
FILEOFF=$(( 0x80 ))   # the single PT_LOAD segment starts at file offset 0x80, vaddr 0x100000
tail -c +$((FILEOFF + 1)) "$REPO/disc/SLUS_204.07" > "$TEXT"

mips-linux-gnu-objdump -D -b binary -m mips:5900 -EL --adjust-vma=0x100000 \
    --start-address=$((ADDR)) --stop-address=$((ADDR + SIZE)) "$TEXT" |
    awk '/^ +[0-9a-f]+:/{ $1=""; print }' > "$TEXT.retail"
mips-linux-gnu-objdump -d -r -m mips:5900 --disassemble="$NAME" "$OBJ" |
    awk '/^ +[0-9a-f]+:/{ $1=""; print } /R_MIPS/{ print "        " $2 " " $3 }' > "$TEXT.ours"
diff -y -W 160 "$TEXT.retail" "$TEXT.ours"
rm -f "$TEXT" "$TEXT.retail" "$TEXT.ours"
