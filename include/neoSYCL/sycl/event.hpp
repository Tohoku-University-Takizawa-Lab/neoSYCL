#ifndef CUSTOM_SYCL_INCLUDE_SYCL_EVENT_HPP_
#define CUSTOM_SYCL_INCLUDE_SYCL_EVENT_HPP_

#include "neoSYCL/sycl/types.hpp"
#include "neoSYCL/sycl/exception.hpp"

namespace neosycl::sycl {

class event {
public:
  event() {}

  ~event() {}

  vector_class <event> get_wait_list() {
    throw unimplemented();
  }

  void wait() {
    throw unimplemented();
  }

  static void wait(const vector_class <event> &eventList) {
    throw unimplemented();
  }

  void wait_and_throw() {
    throw unimplemented();
  }

  static void wait_and_throw(const vector_class <event> &eventList) {
    throw unimplemented();
  }

};

}

#endif //CUSTOM_SYCL_INCLUDE_SYCL_EVENT_HPP_
