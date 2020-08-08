# GpCore2
C++ common utilities and types library

# How to build

## Environment for compiling

- cmake 3.16 (For Ubuntu 18.04 nedeed [installing the latest CMake](https://graspingtech.com/upgrade-cmake/))
- standard for the C++ - 20
- gcc-10, g++-10

```sh
sudo add-apt-repository ppa:ubuntu-toolchain-r/test #for gcc-10 g++-10
sudo apt install cmake gcc-10 g++-10 build-essential libtool libboost-dev
```

## Compile workspace

- create folders **_build/src_**
- go into **_src_** and clone this repo
- go to the repo folder and run

## Linux

```sh
cmake --build . --target all -j4
```

## Wasm32

### Additional tools
- [emscripten](https://emscripten.org/docs/getting_started/downloads.html)
- [jthread](https://github.com/josuttis/jthread.git) - clone to **_build/inc_**

```sh
emcmake cmake . -DBUILD_WASM32=ON 
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
