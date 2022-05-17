#ifndef SYCL_INCLUDE_CL_SYCL_NEC_VE_KERNEL_INFO_HPP_
#define SYCL_INCLUDE_CL_SYCL_NEC_VE_KERNEL_INFO_HPP_

namespace neosycl::sycl::extensions::nec {
struct kernel_info_ve : public detail::kernel_info {
  uint64_t func_;
  uint64_t capt_;
  uint64_t rnge_;

  // capture data and size
  // size_t size_;
  // void* data_;

  kernel_info_ve(const char* c) : detail::kernel_info(c) {
    func_ = 0;
    capt_ = 0;
    rnge_ = 0;
    // size_ = 0;
    // data_ = nullptr;
  }
};
} // namespace neosycl::sycl::extensions::nec

#endif
