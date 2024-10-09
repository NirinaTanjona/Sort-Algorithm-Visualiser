#!/bin/sh

set -xe

CFLAGS="-O3 -Wall -Wextra -ggdb -I./  -I./raylib/include/"
LIBS="./raylib/lib/libraylib.a -lm"

mkdir -p ./build/

clang $CFLAGS -o ./build/visu ./SortVisualizer.c $LIBS && ./build/visu

