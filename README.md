# neoSYCL
SYCL Implementation for CPU / SX-Aurora


## Requirements

CMake >= 3.4

Compiler with C++17 support

## Installation & testing

SYCL is a template library, so no real installation is required.

To build test cases, use following commands:

`cmake -DBUILD_TESTING=ON`

## Examples for CPU

see examples in `examples/cpu`

## Examples for SX-Aurora

To enable SX-Aurora support,  use following commands:

`cmake -DBUILD_VE=ON`

We also need a [sycl-kernel-generator](https://github.com/Tohoku-University-Takizawa-Lab/sycl-kernel-generator) to generate kernel codes.

`cmake -DBUILD_VE=ON -DCMAKE_EXPORT_COMPILE_COMMANDS=ON`  
`clang_tool <your source code> --extra-arg-before=-I<any additional include path>`  
`ncc -shared -fpic -o kernel.so kernel.c`  
`make`