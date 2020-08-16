echo Build x86 shared library ...
g++ -std=c++17 -shared -fPIC -m32 ../src/*.cpp -o shared/posix/libcliac.so -Wl,-out-implib,shared/posix/libcliac.a

echo Build x64 shared library ...
g++ -std=c++17 -shared -fPIC -m64 ../src/*.cpp -o shared/posix/x64/libcliac.so -Wl,-out-implib,shared/posix/x64/libcliac.a

echo Build x86 static library ...
cd static/posix/ || exit
g++ -std=c++17 -m32 -c ../../../src/*.cpp
ar rvs libcliac.a *.o &> /dev/null

echo Build x64 static library ...
cd x64 || exit
g++ -std=c++17 -m64 -c ../../../../src/*.cpp
ar rvs libcliac.a *.o &> /dev/null

echo Remove temporary files ...
cd ../ || exit
find . -type f -name '*.o' -exec rm {} +

echo Copy include directory ...
cd ../../ || exit
cp -rf ../include shared/posix
cp -rf ../include static/posix