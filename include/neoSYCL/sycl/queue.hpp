#ifndef CUSTOM_SYCL_INCLUDE_SYCL_QUEUE_HPP_
#define CUSTOM_SYCL_INCLUDE_SYCL_QUEUE_HPP_

#include <utility>

#include "exception.hpp"
#include "types.hpp"
#include "device_selector.hpp"
#include "info/queue.hpp"
#include "property_list.hpp"
#include "handler.hpp"
#include "platform.hpp"
#include "context.hpp"
#include "detail/task_counter.hpp"

namespace neosycl::sycl {

class queue {
public:
  explicit queue(const property_list &propList = {}) : bind_deice(), counter(new detail::task_counter()) {}

  explicit queue(const async_handler &asyncHandler, const property_list &propList = {});

  explicit queue(const device_selector &deviceSelector, const property_list &propList = {})
      : bind_deice(deviceSelector.select_device()), counter(new detail::task_counter()) {}

  explicit queue(const device_selector &deviceSelector,
                 const async_handler &asyncHandler, const property_list &propList = {});

  explicit queue(const device &syclDevice, const property_list &propList = {}) :
      bind_deice(syclDevice), counter(new detail::task_counter()) {}

  explicit queue(const device &syclDevice, const async_handler &asyncHandler,
                 const property_list &propList = {});

  explicit queue(const context &syclContext, const device_selector &deviceSelector,
                 const property_list &propList = {});

  explicit queue(const context &syclContext, const device_selector &deviceSelector,
                 const async_handler &asyncHandler, const property_list &propList = {});

  explicit queue(const context &syclContext, const device &syclDevice,
                 const property_list &propList = {});

  explicit queue(const context &syclContext, const device &syclDevice,
                 const async_handler &asyncHandler, const property_list &propList = {});

//  explicit queue(cl_command_queue clQueue, const context &syclContext,
//                 const async_handler &asyncHandler = {});

//  cl_command_queue get() const;

  context get_context() const;

  device get_device() const {
    return bind_deice;
  }

  bool is_host() const {
    return bind_deice.is_host();
  }

  template<info::queue param>
  typename info::param_traits<info::queue, param>::return_type get_info() const;

  template<typename T>
  event submit(T cgf) {
    std::thread t([&]() {
      counter->incr();
      try {
        handler command_group_handler;
        cgf(command_group_handler);
      } catch (...) {
        throw;
      }
      counter->decr();
    });

    return event();
  }

  template<typename T>
  event submit(T cgf, const queue &secondaryQueue);

  void wait() {

  }

  void wait_and_throw();

  void throw_asynchronous();

private:
  device bind_deice;
  shared_ptr_class<detail::task_counter> counter;
};

}

#endif //CUSTOM_SYCL_INCLUDE_SYCL_QUEUE_HPP_
