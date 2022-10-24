#pragma once
#include "neoSYCL/sycl/info/queue.hpp"
#include "neoSYCL/sycl/detail/task_counter.hpp"

namespace neosycl::sycl {

///////////////////////////////////////////////////////////////////////////////
class queue {
public:
  explicit queue(const property_list& propList = {})
      : bind_device(device::get_default_device()),
        counter(new detail::task_counter()), ctx(bind_device), prog(ctx) {}

  explicit queue(const async_handler& asyncHandler,
                 const property_list& propList = {})
      : bind_device(device::get_default_device()),
        counter(new detail::task_counter()), err_handler(asyncHandler),
        ctx(bind_device), prog(ctx) {}

  explicit queue(const device_selector& deviceSelector,
                 const property_list& propList = {})
      : bind_device(deviceSelector.select_device()),
        counter(new detail::task_counter()), ctx(bind_device), prog(ctx) {}

  explicit queue(const device_selector& deviceSelector,
                 const async_handler& asyncHandler,
                 const property_list& propList = {})
      : bind_device(deviceSelector.select_device()),
        counter(new detail::task_counter()), err_handler(asyncHandler),
        ctx(bind_device), prog(ctx) {}

  explicit queue(const device& syclDevice, const property_list& propList = {})
      : bind_device(syclDevice), counter(new detail::task_counter()),
        ctx(bind_device), prog(ctx) {}

  explicit queue(const device& syclDevice, const async_handler& asyncHandler,
                 const property_list& propList = {})
      : bind_device(syclDevice), counter(new detail::task_counter()),
        err_handler(asyncHandler), ctx(bind_device), prog(ctx) {}

  explicit queue(const context& syclContext,
                 const device_selector& deviceSelector,
                 const property_list& propList = {});

  explicit queue(const context& syclContext,
                 const device_selector& deviceSelector,
                 const async_handler& asyncHandler,
                 const property_list& propList = {});

  explicit queue(const context& syclContext, const device& syclDevice,
                 const property_list& propList = {});

  explicit queue(const context& syclContext, const device& syclDevice,
                 const async_handler& asyncHandler,
                 const property_list& propList = {});

  //  explicit queue(cl_command_queue clQueue, const context &syclContext,
  //                 const async_handler &asyncHandler = {});

  //  cl_command_queue get() const;

  context get_context() const {
    return ctx;
  }

  device get_device() const {
    return bind_device;
  }

  bool is_host() const {
    return bind_device.is_host();
  }

  template <info::queue param>
  typename info::param_traits<info::queue, param>::return_type get_info() const;

#ifndef DISABLE_MULTI_THREAD_SUPPORT
  template <typename T>
  event submit(T cgf) {
    counter->incr();
    // std::thread t([f = cgf, d = bind_device, p = prog, c = counter]() {
    try {
      handler command_group_handler(bind_device, prog, counter);
      cgf(command_group_handler);
    }
    catch (std::exception& e) {
      PRINT_ERR("%s", e.what());
      throw;
    }
    catch (...) {
      PRINT_ERR("unknown exception");
      throw;
    }
    counter->decr();
    //});
    // t.detach();
    return event();
  }
#else
  /* this may run each command group faster but all command groups will be
   * executed sequencially (i.e. no task parallelism) */
  template <typename T>
  event submit(T cgf) {
    try {
      handler command_group_handler(bind_device, prog, counter);
      cgf(command_group_handler);
    }
    catch (std::exception& e) {
      PRINT_ERR("%s", e.what());
      throw;
    }
    catch (...) {
      PRINT_ERR("unknown exception");
      throw;
    }
    return event();
  }
#endif

  template <typename T>
  event submit(T cgf, const queue& secondaryQueue) {
    throw unimplemented();
  }

  void wait() {
    counter->wait();
  }

  void wait_and_throw();

  void throw_asynchronous();

  virtual ~queue() {
    counter->wait();
  }

private:
  device bind_device;
  shared_ptr_class<detail::task_counter> counter;
  async_handler err_handler;
  context ctx;
  program prog;
};

} // namespace neosycl::sycl
