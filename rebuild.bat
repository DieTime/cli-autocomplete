if exist cmake-build rmdir cmake-build /q /s

cmake -DCMAKE_BUILD_TYPE=Release -S . -B .\cmake-build
cmake --build .\cmake-build --config Release

cls
cd tests\Release
tests.exe
cd ..\..\

cd example\Release
example.exe
cd ..\..\

if exist cmake-build rmdir cmake-build /q /s