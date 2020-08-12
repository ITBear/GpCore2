# GpCore2
C++ common utilities and types library

# How to build

## Environment for compiling

- cmake 3.16 (for Ubuntu 18.04 nedeed [installing the latest CMake](https://graspingtech.com/upgrade-cmake/))
- standard for the C++-20
- gcc-10, g++-10

```sh
sudo add-apt-repository ppa:ubuntu-toolchain-r/test #for gcc-10 g++-10
sudo apt install cmake gcc-10 g++-10 build-essential libtool
```

## Compile workspace

- create folders **_build/src_**
- go into **_src_** and clone this repo (use **-b dev** for development version or **-b master** for stable):
```sh
git clone -b dev https://github.com/ITBear/GpCore2.git
```
- go to the repo folder and run

## Linux

```sh
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_SYSTEM_NAME=Linux -DCMAKE_SYSTEM_PROCESSOR=x86_64 -DBOOST_INCLUDE=/usr/include/boost/ -DBUILD_SHARED_LIBS=ON -DBoost_INCLUDE_DIR=/path/to/boost/
make
make install
```

## Wasm32

### Additional tools
- [emscripten](https://emscripten.org/docs/getting_started/downloads.html)

```sh
emcmake cmake . -DBUILD_WASM32=ON -DBoost_INCLUDE_DIR=/path/to/include/boost
emmake make -j4
make install
```

In the folder **build** be created folders:
```
├── bin
├── doc
├── inc - for headers
├── lib - for *.so files
├── src - for sources
└── tmp
```
