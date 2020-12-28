@echo off

rmdir ..\builds\libs\shared\msvc /s /q
rmdir ..\builds\libs\static\msvc /s /q

mkdir ..\builds\libs\shared\msvc\x64
mkdir ..\builds\libs\static\msvc\x64

@call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x86 %*

echo Build x86 shared library ...
cl /EHsc /c /DBUILD_SHARED ..\src\*.c /I..\include\
link /dll /out:libcliac.dll /implib:libcliac.lib *.obj
xcopy /e /i /y "*.dll" ..\builds\libs\shared\msvc\ 
xcopy /e /i /y "*.lib" ..\builds\libs\shared\msvc\ 
del "*.dll" /s /q
del "*.lib" /s /q
del "*.obj" /s /q
del "*.exp" /s /q

echo Build x86 static library ...
cl /EHsc /c ..\src\*.c /I..\include\
lib *.obj /OUT:libcliac.lib
xcopy /e /i /y "*.lib" ..\builds\libs\static\msvc\ 
del "*.lib" /s /q
del "*.obj" /s /q

@call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x64 %* 

echo Build x64 shared library ...
cl /EHsc /c /DBUILD_SHARED ..\src\*.c /I..\include\
link /dll /out:libcliac.dll /implib:libcliac.lib *.obj
xcopy /e /i /y "*.dll" ..\builds\libs\shared\msvc\x64 
xcopy /e /i /y "*.lib" ..\builds\libs\shared\msvc\x64 
del "*.dll" /s /q
del "*.lib" /s /q
del "*.obj" /s /q
del "*.exp" /s /q

echo Build x64 static library ...
cl /EHsc /c ..\src\*.c /I..\include\
lib *.obj /OUT:libcliac.lib
xcopy /e /i /y "*.lib" ..\builds\libs\static\msvc\x64 
del "*.lib" /s /q
del "*.obj" /s /q

echo Copy include directory ...
xcopy /e /i /y ..\include-shared ..\builds\libs\shared\msvc\include
xcopy /e /i /y ..\include-static ..\builds\libs\static\msvc\include