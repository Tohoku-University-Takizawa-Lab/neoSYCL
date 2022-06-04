#pragma once
#include "neoSYCL/sycl/detail/container/data_container.hpp"
#include "neoSYCL/sycl/detail/container/data_container_nd.hpp"
#include "neoSYCL/sycl/detail/container/buffer_container.hpp"

namespace neosycl::sycl {

namespace detail {
class device_impl;
class program_data;
} // namespace detail

class device {
  friend class handler;
  friend class context;
  friend class platform;
  friend class initial_platform_builder;

public:
  device(const device& rhs) = default;
  device(device&& rhs)      = default;
  ~device()                 = default;

  device& operator=(const device& rhs) = default;
  device& operator=(device&& rhs) = default;

  friend bool operator==(const device& lhs, const device& rhs);
  friend bool operator!=(const device& lhs, const device& rhs);
  friend bool operator<(const device& lhs, const device& rhs);

  explicit device() : impl_(nullptr), plt_() {
    *this = device::get_default_device();
  }

  explicit device(cl_device_id deviceId) {
    throw feature_not_supported("OpenCL interop not supported.");
  }

  explicit device(const device_selector& deviceSelector)
      : impl_(nullptr), plt_() {
    *this = deviceSelector.select_device();
  }

  /* -- common interface members -- */
  //  cl_device_id get() const;

  bool is_host() const;

  bool is_cpu() const;

  bool is_gpu() const;

  bool is_accelerator() const;

  platform get_platform() const {
    return plt_;
  }

  template <info::device param>
  typename info::param_traits<info::device, param>::return_type
  get_info() const;

  bool has_extension(const string_class& extension) const;

  // Available only when prop == info::partition_property::partition_equally
  template <info::partition_property prop>
  vector_class<device> create_sub_devices(size_t nbSubDev) const {
    throw unimplemented();
  }

  // Available only when prop == info::partition_property::partition_by_counts
  template <info::partition_property prop>
  vector_class<device>
  create_sub_devices(const vector_class<size_t>& counts) const {
    throw unimplemented();
  }

#if 0
  // Available only when prop ==
  // info::partition_property::partition_by_affinity_domain
  template <info::partition_property prop>
  vector_class<device>
  create_sub_devices(info::affinity_domain affinityDomain) const;
#endif

  static device get_default_device();

  // INTERNAL USE ONLY
  info::device_type type() const;
  detail::program_data* create_program() const;
  shared_ptr_class<detail::device_impl> get_impl() const {
    return impl_;
  }

private:
  // INTERNAL USE ONLY
  void set_platform(platform p) {
    plt_ = p;
  }
  explicit device(detail::device_impl* impl, platform* p = nullptr)
      : impl_(impl), plt_() {
    if (impl == nullptr) {
      DEBUG_INFO("empty device created");
    }
    else if (p != nullptr)
      plt_ = *p;
  }

  shared_ptr_class<detail::device_impl> impl_;
  platform plt_;
};

bool operator==(const device& lhs, const device& rhs) {
  return (lhs.impl_ == rhs.impl_ && lhs.plt_ == rhs.plt_);
}
bool operator!=(const device& lhs, const device& rhs) {
  return !(lhs == rhs);
}
bool operator<(const device& lhs, const device& rhs) {
  return (lhs.impl_ < rhs.impl_);
}
} // namespace neosycl::sycl
