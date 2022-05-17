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
  using handler_type  = shared_ptr_class<detail::task_handler>;
  using counter_type  = shared_ptr_class<detail::task_counter>;
  using kernel_type   = shared_ptr_class<kernel>;
  using accessor_list = std::vector<detail::accessor_info>;

public:
  explicit handler(device dev, counter_type counter, context c)
      : bind_device_(std::move(dev)), counter_(std::move(counter)),
        kernel_(nullptr), ctx_(c) {}

  template <typename KernelName> void copy_capture(KernelName* p) {
    detail::context_info* cinfo = ctx_.get_context_info();

    cinfo->set_capture(kernel_, p, sizeof(KernelName));
  }

  template <typename KernelName, typename KernelType>
  void set_kernel(KernelType kernelFunc) {
    detail::context_info* cinfo = ctx_.get_context_info();

    kernel_ = cinfo->get_kernel<KernelName>();
    // kernel_->name = detail::get_kernel_name_from_class<KernelName>();
    if (bind_device_.type() != detail::VE)
      return;
    kernelFunc();
  }

  template <typename KernelName, typename KernelType>
  void single_task(KernelType kernelFunc) {
    handler_type task_handler = ctx_.get_context_info()->task_handler;
    submit_task([f = kernelFunc, h = task_handler, k = kernel_]() {
      h->single_task(k, f);
    });
  }

  template <typename KernelType>
  void submit_parallel_for(handler_type handler, range<3> numWorkItems,
                           id<3> offset, KernelType kernelFunc) {
    submit_task([f = kernelFunc, n = numWorkItems, o = offset,
                 h = std::move(handler),
                 k = kernel_]() { h->parallel_for_3d(k, n, f, o); });
  }

  template <typename KernelType>
  void submit_parallel_for(handler_type handler, range<2> numWorkItems,
                           id<2> offset, KernelType kernelFunc) {
    submit_task([f = kernelFunc, n = numWorkItems, o = offset,
                 h = std::move(handler),
                 k = kernel_]() { h->parallel_for_2d(k, n, f, o); });
  }

  template <typename KernelType>
  void submit_parallel_for(handler_type handler, range<1> numWorkItems,
                           id<1> offset, KernelType kernelFunc) {
    submit_task([f = kernelFunc, n = numWorkItems, o = offset,
                 h = std::move(handler),
                 k = kernel_]() { h->parallel_for_1d(k, n, f, o); });
  }

  template <typename KernelName, typename KernelType, size_t dimensions>
  void parallel_for(range<dimensions> numWorkItems, KernelType kernelFunc) {
    handler_type task_handler = ctx_.get_context_info()->task_handler;

    task_handler->set_range(kernel_, numWorkItems);
    submit_parallel_for(task_handler, numWorkItems, id<dimensions>(),
                        kernelFunc);
  }

  template <typename KernelName, typename KernelType, size_t dimensions>
  void parallel_for(range<dimensions> numWorkItems,
                    id<dimensions> workItemOffset, KernelType kernelFunc) {
    handler_type task_handler = ctx_.get_context_info()->task_handler;

    task_handler->set_range(kernel_, numWorkItems, workItemOffset);
#if 0
      string_class range_obj = "__" + kernel_->get_name() + "_range__";
      task_handler->set_range(range_obj.c_str(), numWorkItems);
      string_class offset_obj = "__" + kernel_->get_name() + "_offset__";
      task_handler->set_range(offset_obj.c_str(), workItemOffset);
#endif
    submit_parallel_for(task_handler, numWorkItems, workItemOffset, kernelFunc);
  }

  //  template<typename KernelName, typename KernelType, int dimensions>
  //  void parallel_for(nd_range<dimensions> executionRange, KernelType
  //  kernelFunc);

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
    acc_.push_back(detail::accessor_info(acc, m));
  }

  template <typename T, size_t D, access::mode m, access::target t,
            access::placeholder p>
  T* get_pointer(sycl::accessor<T, D, m, t, p> acc) {
    return (T*)ctx_.get_context_info()->get_pointer(acc.data);
  }

  context get_context() { return ctx_; }

  accessor_list& get_acc_() { return acc_; }

private:
  kernel_type kernel_;
  device bind_device_;
  counter_type counter_;
  context ctx_;
  accessor_list acc_;

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
