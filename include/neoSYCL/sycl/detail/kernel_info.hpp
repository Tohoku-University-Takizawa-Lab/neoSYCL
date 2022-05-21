#ifndef SYCL_INCLUDE_CL_SYCL_KERNEL_KERNEL_HPP_
#define SYCL_INCLUDE_CL_SYCL_KERNEL_KERNEL_HPP_

#include <utility>
#include "neoSYCL/sycl/detail/accessor_info.hpp"
#include <dlfcn.h>

namespace neosycl::sycl::detail {

struct kernel_info {
  std::string name;

  virtual ~kernel_info() = default;
protected:
  kernel_info(const char* c) : name(c) {}
};

struct kernel_info_cpu : public kernel_info {
  int (*func_)();
  void* capt_;
  void* rnge_;

  kernel_info_cpu(const char* c) : kernel_info(c) {
    func_ = nullptr;
    capt_ = nullptr;
    rnge_ = nullptr;
  }
};

} // namespace neosycl::sycl::detail

#endif // SYCL_INCLUDE_CL_SYCL_KERNEL_KERNEL_HPP_
