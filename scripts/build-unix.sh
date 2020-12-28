#!/bin/bash

rm -rf ../builds/libs/shared/unix
rm -rf ../builds/libs/static/unix

mkdir -p ../builds/libs/shared/unix/x64/
mkdir -p ../builds/libs/static/unix/x64/

echo Build x86 shared library ...
gcc -shared -fPIC -m32 ../src/*.c -o ../builds/libs/shared/unix/libcliac.so -Wl,-out-implib,../builds/libs/shared/unix/libcliac.a

echo Build x64 shared library ...
gcc -shared -fPIC -m64 ../src/*.c -o ../builds/libs/shared/unix/x64/libcliac.so -Wl,-out-implib,../builds/libs/shared/unix/x64/libcliac.a

echo Build x86 static library ...
cd ../builds/libs/static/unix/
gcc -m32 -c ../../../../src/*.c
ar rvs libcliac.a *.o
find . -type f -name '*.o' -exec rm {} +

echo Build x64 static library ...
cd x64
gcc -m64 -c ../../../../../src/*.c
ar rvs libcliac.a *.o
find . -type f -name '*.o' -exec rm {} +

echo Copy include directory ...
cd ../../../../../scripts
cp -rf ../include-shared/. ../builds/libs/shared/unix/include
cp -rf ../include-static/. ../builds/libs/static/unix/include