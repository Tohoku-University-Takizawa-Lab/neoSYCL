#pragma once

#include "neoSYCL/sycl/info/device_type.hpp"
#include "neoSYCL/sycl/info/device.hpp"
#include "neoSYCL/sycl/info/param_traits.hpp"
#include "neoSYCL/sycl/detail/device_info.hpp"

namespace neosycl::sycl {

class device {
  friend class handler;
  friend class context;
  friend class platform;

public:
  device(const device& rhs) = default;
  device(device&& rhs)      = default;
  ~device()                 = default;

  device& operator=(const device& rhs) = default;
  device& operator=(device&& rhs) = default;

  friend bool operator==(const device& lhs, const device& rhs);
  friend bool operator!=(const device& lhs, const device& rhs);

  device() : info_(nullptr), plt_(nullptr) {
    *this = platform::get_default_platform().get_devices()[0];
  }

  explicit device(detail::device_info* info, platform* p = nullptr)
      : info_(info), plt_(nullptr) {
    if (p != nullptr)
      plt_ = *p;
  }

  explicit device(cl_device_id deviceId) : info_(nullptr), plt_(nullptr) {
    throw feature_not_supported("OpenCL interop not supported.");
  }

  explicit device(const device_selector& deviceSelector)
      : info_(nullptr), plt_(nullptr) {
    *this = deviceSelector.select_device();
  }

  /* -- common interface members -- */
  //  cl_device_id get() const;

  bool is_host() const { return info_->is_host(); }

  bool is_cpu() const { return info_->is_cpu(); }

  bool is_gpu() const { return info_->is_gpu(); }

  bool is_accelerator() const { return info_->is_accelerator(); }

  platform get_platform() const { return plt_; }

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

  info::device_type type() const { return info_->type(); }

  detail::context_info* create_context_info() const {
    return info_->create_context_info(*this);
  }
private:
  void set_platform(platform p) { plt_ = p; }

  shared_ptr_class<detail::device_info> info_;
  platform plt_;
};

bool operator==(const device& lhs, const device& rhs) {
  return (lhs.info_ == rhs.info_ && lhs.plt_ == rhs.plt_);
}
bool operator!=(const device& lhs, const device& rhs) { return !(lhs == rhs); }

vector_class<device> platform::get_devices(info::device_type t) const {
  vector_class<device> ret;
  for (const device& dev : info_->list_devices()) {
    if (t == info::device_type::all || t == dev.type())
      ret.push_back(dev);
  }
  return ret;
}

platform::platform(const device_selector& deviceSelector) {
  device d = deviceSelector.select_device();
  info_    = shared_ptr_class<detail::platform_info>(
      new detail::host_platform_info(d));
  for (auto& dev : info_->dev_)
    dev.set_platform(*this);
}

platform::platform(detail::platform_info* info) : info_(info) {
  /* do nothing if info == null */
  if (info) {
    for (auto& dev : info_->dev_)
      dev.set_platform(*this);
  }
}

#ifndef BUILD_VE
// See also extensions/nec/ve_selector.hpp
platform platform::register_all_devices() {
  device d(new detail::default_device_info());
  // create a platform with the defaut device at first
  platform p(new detail::default_platform_info(d));
  // register all available devices
  return p;
}
#endif

namespace detail {
/* platform with a single device */
platform_info::platform_info(device d) : dev_() { dev_.push_back(d); }
host_platform_info::host_platform_info(device d) : platform_info(d) {}
} // namespace detail

} // namespace neosycl::sycl
