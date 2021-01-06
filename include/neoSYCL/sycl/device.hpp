#ifndef CUSTOM_SYCL_INCLUDE_SYCL_DEVICE_HPP_
#define CUSTOM_SYCL_INCLUDE_SYCL_DEVICE_HPP_

#include "exception.hpp"
#include "types.hpp"
#include "platform.hpp"
#include "info/device_type.hpp"
#include "info/device.hpp"
#include "info/param_traits.hpp"

namespace neosycl::sycl {

class device {
 public:
 public:
  device();

//  explicit device(cl_device_id deviceId);

  explicit device(const device_selector &deviceSelector);

  /* -- common interface members -- */
//  cl_device_id get() const;

  bool is_host() const;

  bool is_cpu() const;

  bool is_gpu() const;

  bool is_accelerator() const;

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
      info::device_type deviceType = info::device_type::all);
};

}

#endif //CUSTOM_SYCL_INCLUDE_SYCL_DEVICE_HPP_
