#ifndef CUSTOM_SYCL_INCLUDE_SYCL_QUEUE_HPP_
#define CUSTOM_SYCL_INCLUDE_SYCL_QUEUE_HPP_

#include <utility>

#include "exception.hpp"
#include "types.hpp"
#include "device_selector.hpp"
#include "info/queue.hpp"
#include "property_list.hpp"
#include "handler.hpp"
#include "handler_event.hpp"
#include "platform.hpp"
#include "context.hpp"
#include "kernel/task.hpp"

namespace neosycl::sycl {

class queue : detail::SharedPtrImplementation<detail::TaskQueue> {
  using base_class = typename detail::SharedPtrImplementation<detail::TaskQueue>;
 private:
  device bind_device;

 public:

  explicit queue(const property_list &propList = {}) : base_class(new detail::TaskQueue()) {}

  explicit queue(const async_handler &asyncHandler, const property_list &propList = {})
      : base_class(new detail::TaskQueue()) {}

  explicit queue(const device_selector &deviceSelector, const property_list &propList = {})
      : base_class(new detail::TaskQueue()) {
  }

  explicit queue(device dev, const property_list &propList = {})
      : base_class(new detail::TaskQueue()), bind_device(std::move(dev)) {
  }

  device get_device() {
    return bind_device;
  }

  bool is_host() {
    return bind_device.is_host();
  }

  void wait() {
    implementation->wait();
  }

  void wait_and_throw() {
    implementation->wait();
  }

  template<info::queue param>
  typename info::param_traits<info::queue, param>::type get_info() const {
    throw UnimplementedException();
  }

  virtual handler_event submit(const std::function<void(handler &)> &cgf) {
    handler command_group_handler(implementation);
    cgf(command_group_handler);
    return handler_event();
  }

  virtual ~queue() {
    implementation->wait();
  }

};

}

#endif //CUSTOM_SYCL_INCLUDE_SYCL_QUEUE_HPP_
