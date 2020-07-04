#ifndef CUSTOM_SYCL_INCLUDE_SYCL_CONTEXT_HPP_
#define CUSTOM_SYCL_INCLUDE_SYCL_CONTEXT_HPP_

#include "CL/SYCL/exception.hpp"
#include "CL/SYCL/info/context.hpp"

namespace cl::sycl {

class context {

 public:
  context() = default;

  explicit context(async_handler errHandler) {
    throw UnimplementedException();
  }

  context(const device_selector &deviceSelector,
          info::gl_context_interop interopFlag,
          async_handler errHandler = nullptr) {
    throw UnimplementedException();
  }

  context(const device &dev,
          info::gl_context_interop interopFlag,
          async_handler errHandler = nullptr) {
    throw UnimplementedException();
  }

  context(const platform &plt,
          info::gl_context_interop interopFlag,
          async_handler errHandler = nullptr) {
    throw UnimplementedException();
  }

  context(const vector_class<device> &deviceList,
          info::gl_context_interop interopFlag,
          async_handler errHandler = nullptr) {
    throw UnimplementedException();
  }

  bool is_host() const {
    return true;
  }

  platform get_platform() {
    throw UnimplementedException();
  }

  vector_class<device> get_devices() const {
    throw UnimplementedException();
  }

  template<info::context Param>
  typename info::param_traits<info::context, Param>::type get_info() const {
    throw UnimplementedException();
  }

};

}

#endif //CUSTOM_SYCL_INCLUDE_SYCL_CONTEXT_HPP_
