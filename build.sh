#!/bin/sh

set -xe

mkdir -p bin
cp lib/glew32.dll bin/glew32.dll
gcc src/*.c -o bin/simulation -Iinclude -Isrc -Llib -lmingw32 -lglfw3 -lopengl32 -lgdi32 -luser32 -lglew32 -Wall -Wextra