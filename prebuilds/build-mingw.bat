@echo off

echo Build x86 shared library ...
g++ -std=c++17 -shared -m32 -DBUILD_DLL ../src/*.cpp -o windows-mingw/libcliac.dll -Wl,-out-implib,windows-mingw/libcliac.a

echo Build x64 shared library ...
g++64 -std=c++17 -shared -m64 -DBUILD_DLL ../src/*.cpp -o windows-mingw/x64/libcliac.dll -Wl,-out-implib,windows-mingw/x64/libcliac.a