//
// Created by WhiteBlue on 2020/5/25.
//

#ifndef CUSTOM_SYCL_INCLUDE_SYCL_INFO_PARAM_TRAITS_HPP_
#define CUSTOM_SYCL_INCLUDE_SYCL_INFO_PARAM_TRAITS_HPP_

#include "CL/SYCL/info/platform.hpp"

namespace cl::sycl::info {

template<typename T, T Param>
class param_traits {
  using return_type = T;
};

template<info::platform Param>
class param_traits<info::platform, Param> {
  using type = string_class;
};

}

#endif //CUSTOM_SYCL_INCLUDE_SYCL_INFO_PARAM_TRAITS_HPP_
