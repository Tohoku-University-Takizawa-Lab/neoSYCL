# neoSYCL

[![build](https://github.com/Tohoku-University-Takizawa-Lab/neoSYCL/actions/workflows/build.yml/badge.svg)](https://github.com/Tohoku-University-Takizawa-Lab/neoSYCL/actions/workflows/build.yml)

SYCL Implementation for CPU and NEC SX-Aurora TSUBASA (SX-AT)

## Requirements

- CMake 3.10 or higher
- A C++ compiler supporting C++17
- Clang and LLVM 12 or higher (if using the `kout` kernel outliner)
- Python 3 (if using the `nsc++` compiler wrapper)

We regularly test on Rocky Linux 8 and Ubuntu 20.04 on our GitHub Actions
pipeline. You may use the following commands to install the required
dependencies on these distros:

- Rocky Linux 8: `yum install -y git gcc gcc-c++ cmake make clang-devel llvm-devel python3 libffi-devel`
- Ubuntu 20.04: `apt-get install -y git gcc g++ cmake make pkg-config clang-12 libclang-12-dev llvm-12-dev python3 libffi-dev`

## Installation

```
mkdir build && cd build
cmake -DBUILD_VE=ON -DBUILD_KERNEL_GENERATOR=ON ..
make
sudo make install
```

- To enable the SX-Aurora TSUBASA support, add the `-DBUILD_VE=ON` option when
    invoking CMake.
- If Clang and LLVM are installed to non-standard locations, their paths
  should be given via the options `-DCLang_DIR=` and `-DLLVM_DIR=` when
  invoking CMake.
