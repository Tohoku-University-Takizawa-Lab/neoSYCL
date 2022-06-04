#pragma once

namespace neosycl::sycl::extensions::nec {

class kernel_data_ve : public detail::kernel_data {
public:
  uint64_t func_;
  uint64_t capt_;
  uint64_t rnge_;
  struct veo_args* argp_;

  kernel_data_ve() : kernel_data() {
    func_ = 0;
    capt_ = 0;
    rnge_ = 0;
    argp_ = veo_args_alloc();
  }

  ~kernel_data_ve() {
    veo_args_free(argp_);
  }
};
} // namespace neosycl::sycl::extensions::nec
