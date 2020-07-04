#ifndef SYCL_INCLUDE_CL_SYCL_KERNEL_KERNEL_ARG_HPP_
#define SYCL_INCLUDE_CL_SYCL_KERNEL_KERNEL_ARG_HPP_

namespace cl::sycl::detail {

struct KernelArg {

  std::shared_ptr<detail::DataContainer> container;
  access::mode mode;

  KernelArg(std::shared_ptr<detail::DataContainer> arg, access::mode mode) : container(std::move(arg)), mode(mode) {}

  void lock() {
    if (mode == access::mode::read_write || mode == access::mode::write) {
      container->lock_write();
    } else {
      container->lock_read();
    }
  }

  void unlock() {
    if (mode == access::mode::read_write || mode == access::mode::write) {
      container->unlock_write();
    } else {
      container->unlock_read();
    }
  }
};

}

#endif //SYCL_INCLUDE_CL_SYCL_KERNEL_KERNEL_ARG_HPP_
