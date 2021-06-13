#ifndef NEOSYCL_INCLUDE_NEOSYCL_SYCL_DETAIL_DEVICE_INFO_HPP
#define NEOSYCL_INCLUDE_NEOSYCL_SYCL_DETAIL_DEVICE_INFO_HPP

#include "neoSYCL/sycl/detail/device_type.hpp"

namespace neosycl::sycl::detail {

struct device_info {
  virtual bool is_host() = 0;

  virtual bool is_cpu() = 0;

  virtual bool is_gpu() = 0;

  virtual bool is_accelerator() = 0;

  virtual SUPPORT_PLATFORM_TYPE type() = 0;

};

struct cpu_device_info : public device_info {
  bool is_host() override {
    return true;
  }

  bool is_cpu() override {
    return true;
  }
  bool is_gpu() override {
    return false;
  }
  bool is_accelerator() override {
    return false;
  }

  SUPPORT_PLATFORM_TYPE type() override {
    return SUPPORT_PLATFORM_TYPE::CPU;
  }
};

using default_device_info = cpu_device_info;

}

#endif //NEOSYCL_INCLUDE_NEOSYCL_SYCL_DETAIL_DEVICE_INFO_HPP
