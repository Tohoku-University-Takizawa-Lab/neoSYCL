#pragma once

#include "neoSYCL/sycl/property_list.hpp"
#include "neoSYCL/sycl/info/context.hpp"
//#include "neoSYCL/sycl/detail/context_info.hpp"

namespace neosycl::sycl {

namespace detail {
class context_impl;
}

class context {

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

  context(cl_context clContext, async_handler asyncHandler = {}) {
    throw unimplemented();
  }

  template <info::context param>
  typename info::param_traits<info::context, param>::return_type
  get_info() const;

  vector_class<device> get_devices() const;

private:
  void init(vector_class<device>);

  shared_ptr_class<detail::context_impl> impl_;
};

} // namespace neosycl::sycl
