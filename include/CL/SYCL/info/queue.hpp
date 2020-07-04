//
// Created by WhiteBlue on 2020/5/25.
//

#ifndef CUSTOM_SYCL_INCLUDE_SYCL_INFO_QUEUE_HPP_
#define CUSTOM_SYCL_INCLUDE_SYCL_INFO_QUEUE_HPP_

namespace cl::sycl::info {

using queue_profiling = bool;


enum class queue : int {
  context,
  device,
  reference_count,
  properties
};

}

#endif //CUSTOM_SYCL_INCLUDE_SYCL_INFO_QUEUE_HPP_
