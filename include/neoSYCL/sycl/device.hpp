#ifndef CUSTOM_SYCL_INCLUDE_SYCL_DEVICE_HPP_
#define CUSTOM_SYCL_INCLUDE_SYCL_DEVICE_HPP_

#include "neoSYCL/sycl/exception.hpp"
#include "neoSYCL/sycl/types.hpp"
#include "neoSYCL/sycl/platform.hpp"
#include "neoSYCL/sycl/info/device_type.hpp"
#include "neoSYCL/sycl/info/device.hpp"
#include "neoSYCL/sycl/info/param_traits.hpp"
#include "neoSYCL/sycl/detail/device_info.hpp"

namespace neosycl::sycl {

class device {
  friend class handler;

public:
  device() : device_info(new detail::default_device_info()) {};

  device(const shared_ptr_class<detail::device_info> &info) : device_info(info) {}

//  explicit device(cl_device_id deviceId);

  explicit device(const device_selector &deviceSelector) {};

  /* -- common interface members -- */
//  cl_device_id get() const;

  bool is_host() const {
    return device_info->is_host();
  }

  bool is_cpu() const {
    return device_info->is_cpu();
  }

  bool is_gpu() const {
    return device_info->is_gpu();
  }

  bool is_accelerator() const {
    return device_info->is_accelerator();
  }

  platform get_platform() const;
  template<info::device param>
  typename info::param_traits<info::device, param>::return_type get_info() const;

  bool has_extension(const string_class &extension) const;

// Available only when prop == info::partition_property::partition_equally
  template<info::partition_property prop>
  vector_class<device> create_sub_devices(size_t nbSubDev) const;

// Available only when prop == info::partition_property::partition_by_counts
  template<info::partition_property prop>
  vector_class<device> create_sub_devices(const vector_class<size_t> &counts) const;

// Available only when prop == info::partition_property::partition_by_affinity_domain
//  template<info::partition_property prop>
//  vector_class<device> create_sub_devices(info::affinity_domain affinityDomain) const;

  static vector_class<device> get_devices(
      info::device_type deviceType = info::device_type::all) {
    vector_class<device> ret;
    for (const platform &info: platform::get_platforms()) {
      for (const device &dev:info.get_devices()) {
        ret.push_back(dev);
      }
    }
    return ret;
  }

private:
  shared_ptr_class<detail::device_info> device_info;
};

device device_selector::select_device() const {
  return device();
}

vector_class<device> platform::get_devices(info::device_type) const {
  vector_class<device> ret;
  for (shared_ptr_class<detail::device_info> info:platform_info->list_devices()) {
    ret.push_back(device(info));
  }
  return ret;
}

}

#endif //CUSTOM_SYCL_INCLUDE_SYCL_DEVICE_HPP_
