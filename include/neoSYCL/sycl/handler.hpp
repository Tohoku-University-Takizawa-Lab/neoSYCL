#pragma once

//#include <utility>
//#include <shared_mutex>
//#include <regex>
#include "neoSYCL/sycl/nd_range.hpp"
//#include "neoSYCL/sycl/types.hpp"
//#include "neoSYCL/sycl/id.hpp"
//#include "neoSYCL/sycl/allocator.hpp"
//#include "neoSYCL/sycl/detail/highlight_func.hpp"
//#include "neoSYCL/sycl/kernel.hpp"
//#include "neoSYCL/sycl/detail/task.hpp"
//#include "neoSYCL/sycl/detail/task_handler.hpp"
#include "neoSYCL/sycl/detail/task_counter.hpp"
#include "neoSYCL/sycl/detail/handler.hpp"

namespace neosycl::sycl {

class handler {
  using counter_type = shared_ptr_class<detail::task_counter>;
  using kernel_type  = shared_ptr_class<kernel>;
  // using accessor_list = std::vector<detail::accessor_info>;

  friend class queue;
  explicit handler(context c, device d, program p, counter_type counter)
      : ctx_(c), dev_(d), prog_(p), cntr_(std::move(counter)), kernel_() {}

public:
  template <typename KernelName, typename KernelType, size_t dimensions>
  void run(range<dimensions> kernelRange, id<dimensions> kernelOffset,
           KernelType kernelFunc) {
    // handler_type task_handler = cinfo_->task_handler;
    // kernel_                   = cinfo_->get_kernel<KernelName>();
    kernel_ = prog_.get_kernel<KernelName>();
    kernel_.get_acc().clear();
    detail::device_impl* dev = dev_.impl_.get();
    dev->set_range(kernel_, kernelRange, kernelOffset);

    kernelFunc();

    submit_task([h = dev, k = kernel_]() { h->run(k); });
  }

  template <typename KernelName, typename KernelType, size_t dimensions>
  void run(range<dimensions> kernelRange, KernelType kernelFunc) {
    kernel_ = prog_.get_kernel<KernelName>();
    kernel_.get_acc().clear();
    detail::device_impl* dev = dev_.impl_.get();
    dev->set_range(kernel_, kernelRange);

    kernelFunc();

    submit_task([h = dev, k = kernel_]() { h->run(k); });
  }

  template <typename KernelName, typename KernelType>
  void run(KernelType kernelFunc) {
    kernel_ = prog_.get_kernel<KernelName>();
    kernel_.get_acc().clear();
    detail::device_impl* dev = dev_.impl_.get();

    kernelFunc();

    submit_task([h = dev, k = kernel_]() { h->run(k); });
  }

  template <typename KernelName>
  void copy_capture(KernelName* p) {
    dev_.impl_->set_capture(kernel_, p, sizeof(KernelName));
  }

  template <typename KernelName, typename KernelType>
  void single_task(KernelType kernelFunc) {
    if (!dev_.is_host())
      return;
    detail::single_task(kernel_, kernelFunc);
  }

  template <typename KernelName, typename KernelType, size_t dimensions>
  void parallel_for(range<dimensions> numWorkItems, KernelType kernelFunc) {
    if (!dev_.is_host())
      return;
    detail::parallel_for(kernel_, numWorkItems, kernelFunc, id<dimensions>{},
                         get_index_type(kernelFunc));
  }

  template <typename KernelName, typename KernelType, size_t dimensions>
  void parallel_for(range<dimensions> numWorkItems,
                    id<dimensions> workItemOffset, KernelType kernelFunc) {
    if (!dev_.is_host())
      return;
    detail::parallel_for(kernel_, numWorkItems, kernelFunc, workItemOffset,
                         get_index_type(kernelFunc));
  }

  // experimental impl just for translation testing
  template <typename KernelName, typename KernelType, int dimensions>
  void parallel_for(nd_range<dimensions> executionRange,
                    KernelType kernelFunc) {
    PRINT_ERR("Not implemented yet.")
    throw unimplemented();
  }

  template <typename KernelName, typename WorkgroupFunctionType, int dimensions>
  void parallel_for_work_group(range<dimensions> numWorkGroups,
                               WorkgroupFunctionType kernelFunc) {
    PRINT_ERR("Not implemented yet.")
    throw unimplemented();
  }

  template <typename KernelName, typename WorkgroupFunctionType, int dimensions>
  void parallel_for_work_group(range<dimensions> numWorkGroups,
                               range<dimensions> workGroupSize,
                               WorkgroupFunctionType kernelFunc) {
    PRINT_ERR("Not implemented yet.")
    throw unimplemented();
    ;
  }

  //----- OpenCL interoperability interface //
  template <typename T>
  void set_arg(int argIndex, T&& arg);

  template <typename... Ts>
  void set_args(Ts&&... args);

  template <typename T, size_t D, access::mode m, access::target t>
  void require(sycl::accessor<T, D, m, t, access::placeholder::true_t> acc) {
    dev_.impl_->alloc_mem(*acc.data, m);
    // acc.handler_ = this;
    // acc_.push_back(detail::accessor_info(acc, m));
  }

  template <typename T, size_t D, access::mode m, access::target t,
            access::placeholder p>
  T* get_pointer(sycl::accessor<T, D, m, t, p> acc) {
    if (acc.device_ptr)
      return (T*)acc.device_ptr;
    return (T*)dev_.impl_->get_pointer(*acc.data);
  }

  template <typename T, size_t D, access::mode m, access::target t,
            access::placeholder p>
  void* alloc_mem_(sycl::accessor<T, D, m, t, p>& acc) {
    acc.device_ptr = dev_.impl_->alloc_mem(*acc.data, m);
    return acc.device_ptr;
  }

  context& get_context() {
    return ctx_;
  }

  // accessor_list& get_acc_() { return acc_; }

  template <typename T, size_t D, access::mode m, access::target t,
            access::placeholder p>
  neosycl::sycl::rt::acc_<T> map_(sycl::accessor<T, D, m, t, p> acc) {
    size_t sz[6] = {1, 1, 1, 0, 0, 0};
    size_t i;
    for (i = 0; i < kernel_.get_acc().size(); i++)
      if (kernel_.get_acc()[i].data == acc.data) {
        if (m != access::mode::read)
          kernel_.get_acc()[i].mode = m;
        break;
      }
    if (i == kernel_.get_acc().size()) {
      kernel_.get_acc().push_back(detail::accessor_data(acc.data, m));
    }
    std::memcpy(sz + 0, &acc.get_range()[0], sizeof(size_t) * D);
    std::memcpy(sz + 3, &acc.get_offset()[0], sizeof(size_t) * D);
    return neosycl::sycl::rt::acc_<T>{
        get_pointer(acc), {sz[0], sz[1], sz[2], sz[3], sz[4], sz[5]}};
  }

private:
  context ctx_;
  device dev_;
  program prog_;
  counter_type cntr_;
  kernel kernel_;
  // detail::context_info* cinfo_;
  // accessor_list acc_;

  template <typename Func>
  void submit_task(Func func) {
    cntr_->incr();
    std::thread t([f = func, c = cntr_]() {
      try {
        f();
      }
      catch (...) {
        throw;
      }
      c->decr();
    });
    t.detach();
  }

  template <typename F, typename retT, typename argT>
  auto index_type_ptr(retT (F::*)(argT)) {
    return (argT*)nullptr;
  }

  template <typename F, typename retT, typename argT>
  auto index_type_ptr_(retT (F::*)(argT) const) {
    return (argT*)nullptr;
  }

  template <typename KernelType>
  auto get_index_type(const KernelType&) {
    return index_type_ptr_(&KernelType::operator());
  }
};

// called by accessor::accessor()
template <typename T, size_t D, access::mode m, access::target t,
          access::placeholder p>
void accessor<T, D, m, t, p>::alloc_(handler& h) {
  h.alloc_mem_(*this);
}
} // namespace neosycl::sycl
