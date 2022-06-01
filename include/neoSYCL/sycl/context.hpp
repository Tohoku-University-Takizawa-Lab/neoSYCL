#pragma once

#include "neoSYCL/sycl/info/context.hpp"
#include "neoSYCL/sycl/detail/context_info.hpp"

namespace neosycl::sycl {
namespace detail {
class backend {
public:
  vector_class<device> dev_;
  vector_class<shared_ptr_class<context_info>> cinfo_;
};
} // namespace detail

class context {
  using container_ptr = detail::accessor_info::container_ptr;

public:
  explicit context(const property_list& propList = {}) { init({}); }

  ~context() = default;

  context(async_handler asyncHandler, const property_list& propList = {});

  context(const device& dev, const property_list& propList = {}) {
    init({dev});
  }

  context(const device& dev, async_handler asyncHandler,
          const property_list& propList = {});

  context(const platform& plt, const property_list& propList = {});

  context(const platform& plt, async_handler asyncHandler,
          const property_list& propList = {});

  context(const vector_class<device>& deviceList,
          const property_list& propList = {});

  context(const vector_class<device>& deviceList, async_handler asyncHandler,
          const property_list& propList = {});

  //  context(cl_context clContext, async_handler asyncHandler = {});

  template <info::context param>
  typename info::param_traits<info::context, param>::return_type
  get_info() const;

  detail::context_info* get_context_info(device d) {
    for (size_t i(0); i < impl_->dev_.size(); i++)
      if (impl_->dev_[i] == d)
        return impl_->cinfo_[i].get();
    DEBUG_INFO("device not found");
    return nullptr;
  }

  void free_mem_(container_ptr c) {
    for (auto& ci : impl_->cinfo_)
      ci->free_mem(c);
  }

private:
  void init(vector_class<device> dev) {
    impl_ = shared_ptr_class<detail::backend>(new detail::backend());
    for (size_t i(0); i < dev.size(); i++) {
      detail::context_info* p = dev[i].create_context_info();
      if (p && p->is_valid()) {
        impl_->dev_.push_back(dev[i]);
        impl_->cinfo_.push_back(shared_ptr_class<detail::context_info>(p));
      }
      else if (p)
        delete p;
    }
    if (dev.size() > 0 && impl_->dev_.size() == 0) {
      DEBUG_INFO("no available device found");
      throw sycl::runtime_error("no available device found");
    }
  }

  shared_ptr_class<detail::backend> impl_;
};

} // namespace neosycl::sycl
