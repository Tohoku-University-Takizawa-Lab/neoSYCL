#ifndef NEOSYCL_INCLUDE_NEOSYCL_SYCL_DETAIL_REGISTERED_PLATFORMS_H
#define NEOSYCL_INCLUDE_NEOSYCL_SYCL_DETAIL_REGISTERED_PLATFORMS_H

#include "neoSYCL/sycl/detail/platform_info.hpp"
#include "neoSYCL/sycl/detail/task_handler.hpp"
#include <map>

#ifdef BUILD_VE
#include "neoSYCL/extensions/nec/ve_task_handler.hpp"
#endif

namespace neosycl::sycl::detail {

static shared_ptr_class<platform_info> REGISTERED_PLATFORMS[] = {
    shared_ptr_class<platform_info>(new default_platform_info())};

static std::map<SUPPORT_PLATFORM_TYPE, shared_ptr_class<task_handler>>
    PLATFORM_HANDLER_MAP = {
        {SUPPORT_PLATFORM_TYPE::CPU,
         shared_ptr_class<task_handler>(new task_handler_cpu())},

#ifdef DBUILD_VE
        {SUPPORT_PLATFORM_TYPE::SX_AURORA,
         shared_ptr_class<task_handler>(new task_handler_cpu())}
#endif
};

} // namespace neosycl::sycl::detail

#endif // NEOSYCL_INCLUDE_NEOSYCL_SYCL_DETAIL_REGISTERED_PLATFORMS_H
