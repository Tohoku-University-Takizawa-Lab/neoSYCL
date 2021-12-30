#ifndef NEOSYCL_INCLUDE_NEOSYCL_SYCL_DETAIL_PLATFORM_INFO_HPP
#define NEOSYCL_INCLUDE_NEOSYCL_SYCL_DETAIL_PLATFORM_INFO_HPP

#include "neoSYCL/sycl/detail/device_info.hpp"
#include "neoSYCL/sycl/detail/device_type.hpp"

namespace neosycl::sycl::detail {

struct platform_info {

  virtual bool is_host() = 0;

  virtual SUPPORT_PLATFORM_TYPE type() = 0;

  virtual bool has_extension(const string_class &extension) = 0;

  virtual vector_class<shared_ptr_class<device_info>> list_devices() = 0;
};

struct cpu_platform_info : public platform_info {

  bool is_host() override { return true; }

  SUPPORT_PLATFORM_TYPE type() override { return SUPPORT_PLATFORM_TYPE::CPU; }

  bool has_extension(const string_class &extension) override { return false; }

  vector_class<shared_ptr_class<device_info>> list_devices() override {
    return {shared_ptr_class<device_info>(new cpu_device_info())};
  }
};

using default_platform_info = cpu_platform_info;

} // namespace neosycl::sycl::detail

#endif // NEOSYCL_INCLUDE_NEOSYCL_SYCL_DETAIL_PLATFORM_INFO_HPP
