//
// Created by WhiteBlue on 2020/5/25.
//

#ifndef CUSTOM_SYCL_INCLUDE_SYCL_INFO_CONTEXT_HPP_
#define CUSTOM_SYCL_INCLUDE_SYCL_INFO_CONTEXT_HPP_

namespace cl::sycl::info {

using gl_context_interop = bool;

enum class context : int {
  reference_count,
  num_devices,
  gl_interop
};

}

#endif //CUSTOM_SYCL_INCLUDE_SYCL_INFO_CONTEXT_HPP_
