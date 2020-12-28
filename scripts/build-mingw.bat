@echo off

mkdir ..\builds\libs\shared\mingw\x64
mkdir ..\builds\libs\static\mingw\x64

echo Build x86 shared library ...
gcc86 -shared -m32 -DBUILD_SHARED ..\src\*.c -o ..\builds\libs\shared\mingw\libcliac.dll -Wl,-out-implib,..\builds\libs\shared\mingw\libcliac.a

echo Build x64 shared library ...
gcc64 -shared -m64 -DBUILD_SHARED ..\src\*.c -o ..\builds\libs\shared\mingw\x64\libcliac.dll -Wl,-out-implib,..\builds\libs\shared\mingw\x64\libcliac.a

echo Build x86 static library ...
cd ..\builds\libs\static\mingw\
gcc86 -m32 -c ..\..\..\..\src\*.c
ar rvs libcliac.a *.o
del "*.o" /s /q

echo Build x64 static library ...
cd x64
gcc64 -m64 -c ..\..\..\..\..\src\*.c
ar64 rvs libcliac.a *.o
del "*.o" /s /q

echo Copy include directory ...
cd ..\..\..\..\..\scripts
xcopy /e /i /y ..\include-shared ..\builds\libs\shared\mingw\include
xcopy /e /i /y ..\include-static ..\builds\libs\static\mingw\include