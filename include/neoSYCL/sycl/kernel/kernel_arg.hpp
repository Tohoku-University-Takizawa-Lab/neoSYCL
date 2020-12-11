#ifndef SYCL_INCLUDE_CL_SYCL_KERNEL_KERNEL_ARG_HPP_
#define SYCL_INCLUDE_CL_SYCL_KERNEL_KERNEL_ARG_HPP_

namespace neosycl::sycl::detail {

struct KernelArg {

  std::shared_ptr<detail::DataContainer> container;
  access::mode mode;

  KernelArg(std::shared_ptr<detail::DataContainer> arg, access::mode mode) : container(std::move(arg)), mode(mode) {}

  // @todo currently rely on shared_lock, discard_write will perform same as write
  void lock() const {
    if (mode == access::mode::read_write || mode == access::mode::write || mode == access::mode::discard_write) {
      container->lock_write();
    } else {
      container->lock_read();
    }
  }

  void unlock() const {
    if (mode == access::mode::read_write || mode == access::mode::write || mode == access::mode::discard_write) {
      container->unlock_write();
    } else {
      container->unlock_read();
    }
  }
};

}

#endif //SYCL_INCLUDE_CL_SYCL_KERNEL_KERNEL_ARG_HPP_
