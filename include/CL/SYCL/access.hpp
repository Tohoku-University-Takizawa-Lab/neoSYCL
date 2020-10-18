#ifndef CUSTOM_SYCL_INCLUDE_SYCL_ACCESS_HPP_
#define CUSTOM_SYCL_INCLUDE_SYCL_ACCESS_HPP_

#include "CL/SYCL/range.hpp"

namespace cl::sycl {

namespace access {

enum mode {
  read = 42,
  write,
  read_write,
  discard_write,
};

enum target {
  global_buffer = 2014, //< Just pick a random number...
  constant_buffer,
  local,
  image,
  host_buffer,
  host_image,
  image_array,
  cl_buffer,
  cl_image
};

enum class fence_space : char {
  local_space,
  global_space,
  global_and_local
};

}

}

#endif //CUSTOM_SYCL_INCLUDE_SYCL_ACCESS_HPP_
