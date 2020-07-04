#ifndef CUSTOM_SYCL_INCLUDE_SYCL_EVENT_HPP_
#define CUSTOM_SYCL_INCLUDE_SYCL_EVENT_HPP_

#include "CL/SYCL/types.hpp"
#include "CL/SYCL/exception.hpp"

namespace cl::sycl {

class event {
 public:
  event() {

  }

  ~event() {

  }

  vector_class<event> get_wait_list() {
    throw UnimplementedException();
  }

  void wait() {
    throw UnimplementedException();
  }

  static void wait(const vector_class<event> &eventList) {
    throw UnimplementedException();
  }

  void wait_and_throw() {
    throw UnimplementedException();
  }

  static void wait_and_throw(const vector_class<event> &eventList) {
    throw UnimplementedException();
  }

};

}

#endif //CUSTOM_SYCL_INCLUDE_SYCL_EVENT_HPP_
