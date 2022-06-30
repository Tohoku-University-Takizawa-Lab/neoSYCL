# neoSYCL

[![build](https://github.com/Tohoku-University-Takizawa-Lab/neoSYCL/actions/workflows/ci.yml/badge.svg)](https://github.com/Tohoku-University-Takizawa-Lab/neoSYCL/actions/workflows/ci.yml)

A SYCL Implementation for CPU and NEC SX-Aurora TSUBASA (SX-AT)

## Requirements

- CMake 3.10 or higher
- A C++ compiler supporting C++17
- Clang and LLVM 12 or higher (including development headers and shared
  libraries, required if using the CPU or VE device)
- Python 3 (required if using the CPU or VE device)
- NEC Software Development Kit for Vector Engine (required if using the VE device)

We regularly test on Rocky Linux 8 and Ubuntu 20.04 on our GitHub Actions
pipeline. Use the following commands to install all required dependencies
(except the VE SDK) on these distributions:

- Rocky Linux 8: `dnf install -y git gcc gcc-c++ cmake make clang-devel llvm-devel python3`
- Ubuntu 20.04: `apt install -y git gcc g++ cmake make pkg-config clang-12 libclang-12-dev llvm-12-dev python3`

## Configuration and Installation

```
mkdir build && cd build
cmake ..
make
sudo make install
```

neoSYCL supports three types of devices:

- Host: Kernels are executed the host. This device is always available.
- CPU: Kernels are compiled to a shared library and executed
  on the host CPU. To use this device, add the `-DBUILD_KERNEL_OUTLINER=ON`
  option when invoking CMake.
- VE: Kernels are compiled to a shared library and executed
  on the Vector Engine using Vector Engine Offloading. To use this device, add
  the `-DBUILD_KERNEL_OUTLINER=ON` and `-DBUILD_VE=ON` options when invoking
  CMake.

If Clang, LLVM or VEOS are installed to non-standard locations, their paths
should be given via the options `-DCLang_DIR=`, `-DLLVM_DIR=` and `-DVEO_DIR=`
options, respectively.

## Usage

When using the CPU or VE device, the SYCL source file needs to be compiled
using the `nsc++` compiler wrapper. The target device can be specified using
the `--device` option. Accepted values are ` cpu` or `ve`.
