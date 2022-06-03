#ifndef SYCL_INCLUDE_CL_SYCL_DETAIL_DEBUG_HPP_
#define SYCL_INCLUDE_CL_SYCL_DETAIL_DEBUG_HPP_

#include <cstdio>
#ifdef DEBUG
#define DEBUG_INFO(...)                                                        \
  {                                                                            \
    std::fprintf(stderr, "[DEBUG] ");                                          \
    std::fprintf(stderr, __VA_ARGS__);                                         \
    std::fprintf(stderr, "\n");                                                \
  }
#else
#define DEBUG_INFO(format, ...)
#endif

#define PRINT_ERR(...)                                                         \
  {                                                                            \
    std::fprintf(stderr, "[ERROR] ");                                          \
    std::fprintf(stderr, __VA_ARGS__);                                         \
    std::fprintf(stderr, "\n");                                                \
    std::fprintf(stderr, "[SOURCE LOC] Line %d in %s\n", __LINE__, __FILE__);  \
  }
//    std::fprintf(stderr,"[LOCATION]: Line %d in %s\n", __LINE__, __FILE__);
#endif // SYCL_INCLUDE_CL_SYCL_DETAIL_DEBUG_HPP_
