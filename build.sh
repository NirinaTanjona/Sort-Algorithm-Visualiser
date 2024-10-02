#!/bin/sh

set -xe

CFLAGS="-Wall -Wextra -ggdb"


clang $CFLAGS -I./raylib/raylib-5.0_linux_amd64/src/ -o  AlgoVisualizer main.c ./raylib/raylib-5.0_linux_amd64/src/libraylib.a -lm
