//
// Created by WhiteBlue on 2020/6/22.
//

#ifndef SYCL_INCLUDE_CL_SYCL_DEVICE_SELECTOR_GPU_SELECTOR_H_
#define SYCL_INCLUDE_CL_SYCL_DEVICE_SELECTOR_GPU_SELECTOR_H_

namespace cl::sycl {

class gpu_selector : public device_selector {

 public:
  int operator()(const device &dev) const override {
    if (dev.is_gpu()) {
      return 1;
    }
    return -1;
  }

};

}

#endif //SYCL_INCLUDE_CL_SYCL_DEVICE_SELECTOR_GPU_SELECTOR_H_
