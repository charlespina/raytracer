Build Requirements:
* CMake >= 3.13 - build generation tool
* conan - dependency management tool - `pip install conan`
* ninja - build management tool (parallelization/optimization) - `pip install ninja`

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


Building with ninja:
```
mkdir build && cd build
conan install ..
cmake -B . -GNinja -S ..
```