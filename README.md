Build Requirements:
* CMake >= 3.13 - build generation tool
* conan - dependency management tool

Building on Windows:
```
mkdir build
cd build
conan install ..
cmake -B . -DCMAKE_TOOLCHAIN_FILE=../config/win64.toolchain -S ..
```

Building on Mac:
```
mkdir build
cd build
conan install ..
cmake -B . -G Xcode -S ..
```