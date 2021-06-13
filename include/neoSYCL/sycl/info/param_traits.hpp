#ifndef CUSTOM_SYCL_INCLUDE_SYCL_INFO_PARAM_TRAITS_HPP_
#define CUSTOM_SYCL_INCLUDE_SYCL_INFO_PARAM_TRAITS_HPP_

#include "neoSYCL/sycl/info/platform.hpp"
#include "neoSYCL/sycl/info/device.hpp"

namespace neosycl::sycl::info {

template<typename T, T Param>
class param_traits {
  using return_type = T;
};

template<info::platform Param>
class param_traits<info::platform, Param> {
  using type = string_class;
};

template<info::device Param>
class param_traits<info::device, Param> {
  using type = string_class;
};

}

#endif //CUSTOM_SYCL_INCLUDE_SYCL_INFO_PARAM_TRAITS_HPP_
