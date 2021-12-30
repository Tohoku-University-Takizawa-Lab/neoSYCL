#ifndef NEOSYCL_INCLUDE_NEOSYCL_SYCL_DETAIL_REGISTERED_PLATFORMS_H
#define NEOSYCL_INCLUDE_NEOSYCL_SYCL_DETAIL_REGISTERED_PLATFORMS_H

#include "neoSYCL/sycl/detail/platform_info.hpp"

namespace neosycl::sycl::detail {

static shared_ptr_class<platform_info> REGISTERED_PLATFORMS[] = {
    shared_ptr_class<platform_info>(new default_platform_info())};

} // namespace neosycl::sycl::detail

#endif // NEOSYCL_INCLUDE_NEOSYCL_SYCL_DETAIL_REGISTERED_PLATFORMS_H
