#ifndef SYCL_INCLUDE_CL_SYCL_DETAIL_DEBUG_HPP_
#define SYCL_INCLUDE_CL_SYCL_DETAIL_DEBUG_HPP_

#ifdef DEBUG
#include <iostream>
#define DEBUG_INFO(...)                                                        \
  std::cerr << "[DEBUG] " << printf(__VA_ARGS__) << std::endl
#else
#define DEBUG_INFO(format, ...)
#endif

#define PRINT_INFO(...) std::cerr << "[INFO] " << __VA_ARGS__ << std::endl
#define PRINT_ERR(...) std::cerr << "[ERROR] " << __VA_ARGS__ << std::endl

#endif // SYCL_INCLUDE_CL_SYCL_DETAIL_DEBUG_HPP_
