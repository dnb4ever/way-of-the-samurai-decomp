#!/bin/bash
# get_compilers.sh <dir> -- fetch the stock EE compilers this project builds with.
#
# Both are the unmodified Sony/Cygnus builds that decomp.me distributes. Each
# tarball is checked against the hash recorded here before it is unpacked.
set -eu
DEST=$1
URL=https://github.com/decompme/compilers/releases/download/compilers

fetch() {  # name sha256
    local name=$1 sum=$2
    [ -x "$DEST/$name/bin/ee-gcc" ] && return 0
    mkdir -p "$DEST/$name"
    local tmp="$DEST/$name.tar.xz"
    curl -fsSL -o "$tmp" "$URL/$name.tar.xz"
    echo "$sum  $tmp" | sha256sum -c --quiet
    tar xf "$tmp" -C "$DEST/$name"
    rm -f "$tmp"
    chmod -R u+x "$DEST/$name"
}

# ee-gcc 2.9-ee-991111: Sony SDK libraries
fetch ee-gcc2.9-991111 99cb5db603cd985b50fc6851157020ad69489f2227309ccf825bd43ac7bc19dd
# ee-gcc 2.96-ee-001003-1: newlib (libc) and the game code it reproduces
fetch ee-gcc2.96       0590d2ca9da8f5903889d66761220d14b47a8d14ba987ca53db84a1650a1fd0a
