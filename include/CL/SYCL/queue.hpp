#ifndef CUSTOM_SYCL_INCLUDE_SYCL_QUEUE_HPP_
#define CUSTOM_SYCL_INCLUDE_SYCL_QUEUE_HPP_

#include <utility>

#include "CL/SYCL/exception.hpp"
#include "CL/SYCL/types.hpp"
#include "CL/SYCL/device_selector.hpp"
#include "CL/SYCL/info/queue.hpp"
#include "CL/SYCL/property_list.hpp"
#include "CL/SYCL/handler.hpp"
#include "CL/SYCL/handler_event.hpp"
#include "CL/SYCL/platform.hpp"
#include "CL/SYCL/context.hpp"
#include "CL/SYCL/kernel/task.hpp"

namespace cl::sycl {

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
