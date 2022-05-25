#ifndef CUSTOM_SYCL_INCLUDE_SYCL_HANDLER_HPP_
#define CUSTOM_SYCL_INCLUDE_SYCL_HANDLER_HPP_

#include <utility>
#include <shared_mutex>
#include <regex>
#include "neoSYCL/sycl/nd_range.hpp"
#include "neoSYCL/sycl/types.hpp"
#include "neoSYCL/sycl/event.hpp"
#include "neoSYCL/sycl/id.hpp"
#include "neoSYCL/sycl/allocator.hpp"
//#include "neoSYCL/sycl/detail/highlight_func.hpp"
#include "neoSYCL/sycl/kernel.hpp"
#include "neoSYCL/sycl/detail/task.hpp"
#include "neoSYCL/sycl/detail/task_handler.hpp"
#include "neoSYCL/sycl/detail/registered_platforms.hpp"
#include "neoSYCL/sycl/detail/task_counter.hpp"

namespace neosycl::sycl {

class handler {
  using handler_type = shared_ptr_class<detail::task_handler>;
  using counter_type = shared_ptr_class<detail::task_counter>;
  using kernel_type  = shared_ptr_class<kernel>;
  // using accessor_list = std::vector<detail::accessor_info>;

public:
  explicit handler(device dev, counter_type counter, context c)
      : bind_device_(std::move(dev)), counter_(std::move(counter)),
        kernel_(nullptr), ctx_(c) {}

  template <typename KernelName, typename KernelType, size_t dimensions>
  void run(range<dimensions> r, id<dimensions> o, KernelType kernelFunc) {
    detail::context_info* cinfo = ctx_.get_context_info();
    handler_type task_handler   = cinfo->task_handler;
    kernel_                     = cinfo->get_kernel<KernelName>();
    kernel_->get_acc().clear();
    task_handler->set_range(kernel_, r, o);

    kernelFunc();

    submit_task([h = task_handler, k = kernel_]() { h->run(k); });
  }

  template <typename KernelName, typename KernelType, size_t dimensions>
  void run(range<dimensions> r, KernelType kernelFunc) {
    detail::context_info* cinfo = ctx_.get_context_info();
    handler_type task_handler   = cinfo->task_handler;
    kernel_                     = cinfo->get_kernel<KernelName>();
    kernel_->get_acc().clear();
    task_handler->set_range(kernel_, r);

    kernelFunc();

    submit_task([h = task_handler, k = kernel_]() { h->run(k); });
  }

  template <typename KernelName, typename KernelType>
  void run(KernelType kernelFunc) {
    detail::context_info* cinfo = ctx_.get_context_info();
    handler_type task_handler   = cinfo->task_handler;
    kernel_                     = cinfo->get_kernel<KernelName>();
    kernel_->get_acc().clear();

    kernelFunc();

    submit_task([h = task_handler, k = kernel_]() { h->run(k); });
  }

  template <typename KernelName> void copy_capture(KernelName* p) {
    detail::context_info* cinfo = ctx_.get_context_info();

    cinfo->set_capture(kernel_, p, sizeof(KernelName));
  }

  template <typename KernelName, typename KernelType>
  void single_task(KernelType kernelFunc) {
    PRINT_ERR("This code is not converted yet.")
    abort();
#if 0
    handler_type task_handler = ctx_.get_context_info()->task_handler;
    submit_task([f = kernelFunc, h = task_handler, k = kernel_]() {
      h->single_task(k, f);
    });
#endif
  }

  template <typename KernelName, typename KernelType, size_t dimensions>
  void parallel_for(range<dimensions> numWorkItems, KernelType kernelFunc) {
    PRINT_ERR("This code is not converted yet.")
    abort();
#if 0
    handler_type task_handler = ctx_.get_context_info()->task_handler;

    task_handler->set_range(kernel_, numWorkItems);
    submit_parallel_for(task_handler, numWorkItems, id<dimensions>(),
                        kernelFunc);
#endif
  }

  template <typename KernelName, typename KernelType, size_t dimensions>
  void parallel_for(range<dimensions> numWorkItems,
                    id<dimensions> workItemOffset, KernelType kernelFunc) {
    PRINT_ERR("This code is not converted yet.")
    abort();
#if 0
    handler_type task_handler = ctx_.get_context_info()->task_handler;

    task_handler->set_range(kernel_, numWorkItems, workItemOffset);
      string_class range_obj = "__" + kernel_->get_name() + "_range__";
      task_handler->set_range(range_obj.c_str(), numWorkItems);
      string_class offset_obj = "__" + kernel_->get_name() + "_offset__";
      task_handler->set_range(offset_obj.c_str(), workItemOffset);
    submit_parallel_for(task_handler, numWorkItems, workItemOffset, kernelFunc);
#endif
  }

  // experimental impl just for testing
  template <typename KernelName, typename KernelType, int dimensions>
  void parallel_for(nd_range<dimensions> executionRange,
                    KernelType kernelFunc) {
    PRINT_ERR("Not implemented yet.")
    throw unimplemented();
  }

  template <typename KernelName, typename WorkgroupFunctionType, int dimensions>
  void parallel_for_work_group(range<dimensions> numWorkGroups,
                               WorkgroupFunctionType kernelFunc);

  template <typename KernelName, typename WorkgroupFunctionType, int dimensions>
  void parallel_for_work_group(range<dimensions> numWorkGroups,
                               range<dimensions> workGroupSize,
                               WorkgroupFunctionType kernelFunc);

  //----- OpenCL interoperability interface //
  template <typename T> void set_arg(int argIndex, T&& arg);

  template <typename... Ts> void set_args(Ts&&... args);

  template <typename T, size_t D, access::mode m, access::target t>
  void require(sycl::accessor<T, D, m, t, access::placeholder::true_t> acc) {
    acc.handler_ = this;
    // acc_.push_back(detail::accessor_info(acc, m));
  }

  template <typename T, size_t D, access::mode m, access::target t,
            access::placeholder p>
  T* get_pointer(sycl::accessor<T, D, m, t, p> acc) {
    return (T*)ctx_.get_context_info()->get_pointer(acc.data);
  }

  context& get_context() { return ctx_; }

  // accessor_list& get_acc_() { return acc_; }

  template <typename T, size_t D, access::mode m, access::target t,
            access::placeholder p>
  neosycl::sycl::rt::acc_<T> map_(sycl::accessor<T, D, m, t, p> acc) {
    size_t sz[6] = {1, 1, 1, 0, 0, 0};
    size_t i;
    for (i = 0; i < kernel_->get_acc().size(); i++)
      if (kernel_->get_acc()[i].container.get() == acc.data.get())
        break;
    if (i == kernel_->get_acc().size()) {
      kernel_->get_acc().push_back(detail::accessor_info(acc.data, m));
    }
    std::memcpy(sz + 0, &acc.get_range()[0], sizeof(size_t) * D);
    std::memcpy(sz + 3, &acc.get_offset()[0], sizeof(size_t) * D);
    return neosycl::sycl::rt::acc_<T>{
        get_pointer(acc), {sz[0], sz[1], sz[2], sz[3], sz[4], sz[5]}};
  }

private:
  device bind_device_;
  counter_type counter_;
  kernel_type kernel_;
  context ctx_;
  // accessor_list acc_;

  template <typename Func> void submit_task(Func func) {
    counter_->incr();
    std::thread t([f = func, c = counter_]() {
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
};

} // namespace neosycl::sycl

#endif // CUSTOM_SYCL_INCLUDE_SYCL_HANDLER_HPP_
