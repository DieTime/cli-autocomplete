@echo off

echo Build x86 shared library ...
@call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x86 %* > NUL
cl /EHsc /std:c++17 /c /DBUILD_DLL ../src/*.cpp /I ../include/ /Fo./windows-msvc/ > NUL
cd windows-msvc && link /DLL /OUT:libcliac.dll *.obj > NUL && cd ../

echo Build x64 shared library ...
@call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x64 %* > NUL
cl /EHsc /std:c++17 /c /DBUILD_DLL ../src/*.cpp /I ../include/ /Fo./windows-msvc/x64/ > NUL
cd windows-msvc/x64/ && link /DLL /OUT:libcliac.dll *.obj > NUL && cd ../../

echo Remove temporary files ...
DEL ".\windows-msvc\*.obj" /S /Q > NUL
DEL ".\windows-msvc\*.exp" /S /Q > NUL