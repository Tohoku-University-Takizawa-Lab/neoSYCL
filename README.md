# neoSYCL
SYCL Implementation for CPU / NEC SX-Aurora TSUBASA (SX-AT)


## Installation & testing

SYCL is a template library, so no real installation is required.

For CPU computation, the runtime library is implemented with pure C++ STL, does not rely on any third-party libraries.

To build examples, use following commands:

`cmake ${CMAKE_FILE_DIR}`


## Enable NEC SX-Aurora TSUBASA (SX-AT) support

@TODO Current SX-AURORA version is under development, please check branch backup.


We also need a `sycl-kernel-generator` to generate kernel codes, which is a module of neoSYCL project.

To enable SX-Aurora support,  use following commands:

`cmake -DBUILD_VE=ON -DClang_DIR=${LLVM_PROJECT_BUILD_DIR}/lib/cmake/clang -DLLVM_DIR=${LLVM_PROJECT_BUILD_DIR}/lib/cmake/llvm -DBUILD_KERNEL_GENERATOR=ON ${CMAKE_FILE_DIR}`

