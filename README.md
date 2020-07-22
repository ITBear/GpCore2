# GpCore2
C++ common utilities and types library

# Build with cmake
Cmake created folders

`../lib` - for \*.so file

`../inc` - for headers

## Environment for compiling
1. cmake 3.10
2. standard for the C++ - 20
3. gcc-10, g++-10
4. emscripten latest

### Linux_x86-64

```cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_SYSTEM_NAME=Linux -DCMAKE_SYSTEM_PROCESSOR=x86_64 -DBOOST_INCLUDE=/usr/include/boost/```

### Wasm_32

```emcmake cmake -DCMAKE_BUILD_TYPE=Release -DEMSCRIPTEN_ROOT_PATH=/path/to/emsdk/upstream/emscripten -DCMAKE_SYSTEM_NAME=Emscripten -DCMAKE_SYSTEM_PROCESSOR=wasm32 -DBOOST_INCLUDE=/usr/include/boost/ -DBUILD_SHARED_LIBS=OFF```
