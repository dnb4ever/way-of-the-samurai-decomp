#!/bin/bash
# build.sh -- clean rebuild, per-function byte check, image byte gate, rule check.
#
#   tools/build.sh                 full build: every unit in config/units.txt
#   tools/build.sh src/x/y.c ...   compile and check only these units (inner loop)
#   tools/build.sh -n [units]      also compile NON_MATCHING bodies and report them
#                                  (with the units of config/parked.txt, whose
#                                  functions are all parked)
#
# Runs on Linux or WSL. The old EE compilers are 32-bit programs that cannot
# stat files on a Windows drive mounted under /mnt, so when the checkout lives
# there the sources are mirrored to a Linux-native directory first.
#
# Inputs:  config/units.txt, config/symbols*.txt, config/SLUS_204.07.sha1,
#          disc/SLUS_204.07 (see README; extract it with tools/extract_elf)
# Outputs: build/  (objects, fncheck.txt, rules.txt, summary.txt, report.json)
set -u -o pipefail

REPO=$(cd "$(dirname "$0")/.." && pwd)
COMPILERS=${WOTS_COMPILERS:-$HOME/.cache/wots/compilers}
NONMATCHING=0
if [ "${1:-}" = "-n" ]; then NONMATCHING=1; shift; fi

RETAIL=$REPO/disc/SLUS_204.07
SHA1=$(cut -d' ' -f1 "$REPO/config/SLUS_204.07.sha1")
[ -f "$RETAIL" ] || { echo "missing $RETAIL: extract it first (see README)"; exit 2; }

# ---- source tree the compilers can read --------------------------------------
case "$REPO" in
/mnt/*)
    WORK=${WOTS_MIRROR:-$HOME/.cache/wots/mirror/$(echo "$REPO" | tr '/' '_')}
    mkdir -p "$WORK"
    rsync -a --delete "$REPO/include" "$REPO/src" "$WORK/" ;;
*)
    WORK=$REPO ;;
esac

# ---- host tools ---------------------------------------------------------------
HOSTBIN=$HOME/.cache/wots/bin/$(echo "$REPO" | tr "/" "_")
mkdir -p "$HOSTBIN"
for t in fncheck rules extract_elf; do
    if [ ! -x "$HOSTBIN/$t" ] || [ "$REPO/tools/$t.c" -nt "$HOSTBIN/$t" ]; then
        cc -O2 -Wall -o "$HOSTBIN/$t" "$REPO/tools/$t.c" || exit 2
    fi
done

"$REPO/tools/get_compilers.sh" "$COMPILERS" || exit 2

# ---- units ---------------------------------------------------------------------
BUILD=$REPO/build
OBJ=$WORK/build/obj
rm -rf "$OBJ" "$BUILD"
mkdir -p "$OBJ" "$BUILD"

declare -a UNITS PARKED
units_of() { grep -v '^\s*\(#\|$\)' "$1" 2>/dev/null; }
if [ $# -gt 0 ]; then
    for u in "$@"; do
        line=$(awk -v u="$u" '$1==u' "$REPO/config/units.txt")
        pline=$(awk -v u="$u" '$1==u' "$REPO/config/parked.txt" 2>/dev/null)
        if [ -n "$line" ]; then UNITS+=("$line")
        elif [ -n "$pline" ]; then PARKED+=("$pline")
        else echo "$u is not in config/units.txt or config/parked.txt"; exit 2; fi
    done
else
    while IFS= read -r line; do UNITS+=("$line"); done < <(units_of "$REPO/config/units.txt")
    while IFS= read -r line; do PARKED+=("$line"); done < <(units_of "$REPO/config/parked.txt")
fi
[ ${#UNITS[@]} -gt 0 ] || [ $NONMATCHING -eq 1 ] || { echo "no units to build"; exit 2; }

compile() {  # unit compiler flags... -> object path on stdout
    local unit=$1 comp=$2; shift 2
    local out=$OBJ/${unit%.c}.o
    mkdir -p "$(dirname "$out")"
    local cc1
    cc1=$(dirname "$(find "$COMPILERS/$comp" -name cc1 | head -1)")
    (cd "$WORK" && "$COMPILERS/$comp/bin/ee-gcc" -B "$cc1/" -B "$COMPILERS/$comp/bin/ee-" \
        -c -nostdinc -Iinclude -Iinclude/gcc "$@" ${EXTRA_CFLAGS:-} "$unit" -o "$out") >&2 || return 1
    echo "$out"
}

OBJS=()
FAILED=0
for line in "${UNITS[@]}"; do
    read -r unit comp flags <<<"$line"
    o=$(compile "$unit" "$comp" $flags) || { echo "COMPILE FAILED: $unit" >&2; FAILED=1; continue; }
    OBJS+=("$o")
done
[ $FAILED -eq 0 ] || { echo "build failed"; exit 1; }

MAPS=(-s "$REPO/config/symbols.txt" -s "$REPO/config/symbols_debug.txt")

# ---- per-function check and the byte gate ---------------------------------------
GATE=0
if [ ${#OBJS[@]} -gt 0 ]; then
    "$HOSTBIN/fncheck" -H "$SHA1" -t "$RETAIL" "${MAPS[@]}" "${OBJS[@]}" > "$BUILD/fncheck.txt"
    GATE=$?
    grep -v '^MATCH ' "$BUILD/fncheck.txt"
else
    : > "$BUILD/fncheck.txt"
fi

# ---- rules ------------------------------------------------------------------------
SOURCES=()
for line in "${UNITS[@]}" "${PARKED[@]}"; do read -r unit _ <<<"$line"; SOURCES+=("$REPO/$unit"); done
HEADERS=$(find "$REPO/include" -name '*.h' -not -path '*/include/gcc/*')
"$HOSTBIN/rules" -m "$BUILD/fncheck.txt" -j "$BUILD/report.json" -T "$REPO/config/totals.txt" \
    "${SOURCES[@]}" $HEADERS > "$BUILD/rules.txt"
grep -E '^(MATCH-PENDING|matched under|bytes matched|[0-9]+ \[match\])' "$BUILD/rules.txt" | tee "$BUILD/summary.txt"

# ---- parked bodies -------------------------------------------------------------------
if [ $NONMATCHING -eq 1 ]; then
    OBJ=$WORK/build/obj-nonmatching
    rm -rf "$OBJ"; mkdir -p "$OBJ"
    NM=()
    for line in "${UNITS[@]}" "${PARKED[@]}"; do
        read -r unit comp flags <<<"$line"
        o=$(EXTRA_CFLAGS=-DNON_MATCHING compile "$unit" "$comp" $flags) && NM+=("$o")
    done
    "$HOSTBIN/fncheck" -t "$RETAIL" "${MAPS[@]}" "${NM[@]}" > "$BUILD/nonmatching.txt"
    grep -v '^MATCH ' "$BUILD/nonmatching.txt" | sed 's/^/NON_MATCHING /'
fi

exit $GATE
