#ifndef CUSTOM_SYCL_INCLUDE_SYCL_CONTEXT_HPP_
#define CUSTOM_SYCL_INCLUDE_SYCL_CONTEXT_HPP_

#include "neoSYCL/sycl/exception.hpp"
#include "neoSYCL/sycl/info/context.hpp"
#include "neoSYCL/sycl/property_list.hpp"

namespace neosycl::sycl {

class context {

public:
  explicit context(const property_list &propList = {});

  context(async_handler asyncHandler, const property_list &propList = {});

  context(const device &dev, const property_list &propList = {});

  context(const device &dev, async_handler asyncHandler,
          const property_list &propList = {});

  context(const platform &plt, const property_list &propList = {});

  context(const platform &plt, async_handler asyncHandler,
          const property_list &propList = {});

  context(const vector_class<device> &deviceList,
          const property_list &propList = {});

  context(const vector_class<device> &deviceList, async_handler asyncHandler,
          const property_list &propList = {});

  //  context(cl_context clContext, async_handler asyncHandler = {});

  template <info::context param>
  typename info::param_traits<info::context, param>::return_type
  get_info() const;
};

} // namespace neosycl::sycl

#endif // CUSTOM_SYCL_INCLUDE_SYCL_CONTEXT_HPP_
