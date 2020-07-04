#ifndef SYCL_INCLUDE_CL_SYCL_DETAIL_DEBUG_HPP_
#define SYCL_INCLUDE_CL_SYCL_DETAIL_DEBUG_HPP_

#ifdef DEBUG
#include <iostream>
#include <fmt/core.h>
#define DEBUG_INFO(...) std::cout<< "[DEBUG] "<< fmt::format(__VA_ARGS__) <<std::endl
#else
#define DEBUG_INFO(format, ...)
#endif

#endif //SYCL_INCLUDE_CL_SYCL_DETAIL_DEBUG_HPP_
