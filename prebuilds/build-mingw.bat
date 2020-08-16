@echo off

echo Build x86 shared library ...
g++ -std=c++17 -shared -m32 -DBUILD_DLL ..\src\*.cpp -o shared\windows-mingw\libcliac.dll -Wl,-out-implib,shared\windows-mingw\libcliac.a

echo Build x64 shared library ...
g++64 -std=c++17 -shared -m64 -DBUILD_DLL ..\src\*.cpp -o shared/windows-mingw\x64\libcliac.dll -Wl,-out-implib,shared\windows-mingw\x64\libcliac.a

echo Build x86 static library ...
cd static\windows-mingw\
g++ -std=c++17 -m32 -DBUILD_STATIC -c ..\..\..\src\*.cpp
ar rvs libcliac.a *.o > NUL

echo Build x64 static library ...
cd x64
g++64 -std=c++17 -m64 -DBUILD_STATIC -c ..\..\..\..\src\*.cpp
ar64 rvs libcliac.a *.o > NUL

echo Remove temporary files ...
cd ..\
DEL ".\*.o" /S /Q > NUL

echo Copy include directory ...
cd ..\..\
xcopy /e /i /y ..\include static\windows-mingw\include > NUL
xcopy /e /i /y ..\include shared\windows-mingw\include > NUL