#pragma once

namespace neosycl::sycl::detail {

class kernel_data_cpu : public kernel_data {
public:
  // void* dll_;
  int (*func_)();
  void* capt_;
  void* rnge_;

  kernel_data_cpu() : kernel_data() {
    // dll_  = nullptr;
    func_ = nullptr;
    capt_ = nullptr;
    rnge_ = nullptr;
  }
};

} // namespace neosycl::sycl::detail
