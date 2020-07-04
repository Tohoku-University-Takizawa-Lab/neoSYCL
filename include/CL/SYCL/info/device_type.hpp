//
// Created by WhiteBlue on 2020/5/25.
//

#ifndef CUSTOM_SYCL_INCLUDE_SYCL_INFO_DEVICE_TYPE_HPP_
#define CUSTOM_SYCL_INCLUDE_SYCL_INFO_DEVICE_TYPE_HPP_

namespace cl::sycl::info {

enum class device_type : unsigned int {
  cpu,
  gpu,
  accelerator,
  custom,
  defaults,
  host,
  all
};


}

#endif //CUSTOM_SYCL_INCLUDE_SYCL_INFO_DEVICE_TYPE_HPP_
