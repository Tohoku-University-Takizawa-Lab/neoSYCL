#ifndef NEOSYCL_INCLUDE_NEOSYCL_SYCL_DETAIL_REGISTERED_PLATFORMS_H
#define NEOSYCL_INCLUDE_NEOSYCL_SYCL_DETAIL_REGISTERED_PLATFORMS_H

#include "platform_info.hpp"
#include "task_handler.hpp"
#include <map>

namespace neosycl::sycl::detail {

static shared_ptr_class<platform_info> REGISTERED_PLATFORMS[] = {
    shared_ptr_class<platform_info>(new default_platform_info())
};

static std::map<SUPPORT_PLATFORM_TYPE, shared_ptr_class<task_handler>> PLATFORM_HANDLER_MAP = {
    {SUPPORT_PLATFORM_TYPE::CPU, shared_ptr_class<task_handler>(new task_handler_cpu())}
};

}

#endif //NEOSYCL_INCLUDE_NEOSYCL_SYCL_DETAIL_REGISTERED_PLATFORMS_H
