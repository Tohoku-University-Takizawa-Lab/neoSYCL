#ifndef CUSTOM_SYCL_INCLUDE_SYCL_ACCESS_HPP_
#define CUSTOM_SYCL_INCLUDE_SYCL_ACCESS_HPP_

namespace neosycl::sycl {

namespace access {

enum class target {
  global_buffer = 2014,
  constant_buffer,
  local,
  image,
  host_buffer,
  host_image,
  image_array
};

enum class mode {
  read = 1024,
  write,
  read_write,
  discard_write,
  discard_read_write,
  atomic
};

enum placeholder {
  false_t,
  true_t
};

enum class address_space : int {
  global_space,
  local_space,
  constant_space,
  private_space
};

}

}

#endif //CUSTOM_SYCL_INCLUDE_SYCL_ACCESS_HPP_
