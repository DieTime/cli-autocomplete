@echo off

echo Build x86 shared library ...
@call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x86 %* > NUL
cl /EHsc /std:c++17 /c /DBUILD_DLL ..\src\*.cpp /I ..\include\ /Fo.\shared\windows-msvc\ > NUL
cd shared\windows-msvc && link /DLL /OUT:libcliac.dll *.obj > NUL && cd ..\..\

echo Build x64 shared library ...
@call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x64 %* > NUL
cl /EHsc /std:c++17 /c /DBUILD_DLL ..\src\*.cpp /I ..\include\ /Fo.\shared\windows-msvc\x64\ > NUL
cd shared\windows-msvc\x64\ && link /DLL /OUT:libcliac.dll *.obj > NUL && cd ..\..\..\

echo Build x86 static library ...
@call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x86 %* > NUL
cl /EHsc /std:c++17 /c /DBUILD_STATIC ..\src\*.cpp /I ..\include\ /Fo.\static\windows-msvc\ > NUL
cd static\windows-msvc\ && lib *.obj /OUT:libcliac.lib > NUL && cd ..\..\

echo Build x64 static library ...
@call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x64 %* > NUL
cl /EHsc /std:c++17 /c /DBUILD_STATIC ..\src\*.cpp /I ..\include\ /Fo.\static\windows-msvc\x64\ > NUL
cd static\windows-msvc\x64\ && lib *.obj /OUT:libcliac.lib > NUL && cd ..\..\..\

echo Remove temporary files ...
DEL ".\shared\windows-msvc\*.obj" /S /Q > NUL
DEL ".\static\windows-msvc\*.obj" /S /Q > NUL
DEL ".\shared\windows-msvc\*.exp" /S /Q > NUL

echo Copy include directory ...
xcopy /e /i /y ..\include static\windows-msvc\include > NUL
xcopy /e /i /y ..\include shared\windows-msvc\include > NUL