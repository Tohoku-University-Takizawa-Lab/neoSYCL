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

namespace neosycl::sycl {

class queue {
 public:
  explicit queue(const property_list &propList = {});

  explicit queue(const async_handler &asyncHandler, const property_list &propList = {});

  explicit queue(const device_selector &deviceSelector, const property_list &propList = {});

  explicit queue(const device_selector &deviceSelector,
                 const async_handler &asyncHandler, const property_list &propList = {});

  explicit queue(const device &syclDevice, const property_list &propList = {});

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

  device get_device() const;

  bool is_host() const;

  template<info::queue param>
  typename info::param_traits<info::queue, param>::return_type get_info() const;

  template<typename T>
  event submit(T cgf) {

  }

  template<typename T>
  event submit(T cgf, const queue &secondaryQueue);

  void wait();

  void wait_and_throw();

  void throw_asynchronous();

};

}

#endif //CUSTOM_SYCL_INCLUDE_SYCL_QUEUE_HPP_
