#ifndef CUSTOM_SYCL_INCLUDE_SYCL_CONTEXT_HPP_
#define CUSTOM_SYCL_INCLUDE_SYCL_CONTEXT_HPP_

#include "neoSYCL/sycl/info/context.hpp"
#include "neoSYCL/sycl/detail/context_info.hpp"

namespace neosycl::sycl {

class context {

public:
  explicit context(const property_list& propList = {}) { init(device()); }

  ~context() = default;

  context(async_handler asyncHandler, const property_list& propList = {});

  context(const device& dev, const property_list& propList = {}) { init(dev); }

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

  detail::context_info* get_context_info() { return ctx_info.get(); }

private:
  void init(const device& dev) {
    ctx_info =
        shared_ptr_class<detail::context_info>(dev.create_context_info());
  }

  shared_ptr_class<detail::context_info> ctx_info;
};

} // namespace neosycl::sycl

#endif // CUSTOM_SYCL_INCLUDE_SYCL_CONTEXT_HPP_
