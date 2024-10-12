#!/bin/sh

set -xe

cd ./lib/

CFLAGS="-Wall -Wextra -I ../include"
clang -c *.c $CFLAGS
ar rcs libalgo.a *.o
cd ../



CFLAGS="-O3 -Wall -Wextra -ggdb -I ./include  -I ./raylib/include"
LIBS="./raylib/lib/libraylib.so -lraylib -L./lib -lalgo -lm -ldl -lpthread"

mkdir -p ./build/

clang  -o ./build/visu ./sort_visualizer.c $CFLAGS $LIBS

cd ./build/
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:../raylib/lib/

./visu


