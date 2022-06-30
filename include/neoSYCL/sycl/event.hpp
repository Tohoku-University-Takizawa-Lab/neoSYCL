#pragma once

namespace neosycl::sycl {

class event {
public:
  event() {}

  ~event() {}

  vector_class<event> get_wait_list() {
    throw unimplemented();
  }

  void wait() {
    throw unimplemented();
  }

  static void wait(const vector_class<event>& eventList) {
    throw unimplemented();
  }

  void wait_and_throw() {
    throw unimplemented();
  }

  static void wait_and_throw(const vector_class<event>& eventList) {
    throw unimplemented();
  }
};

} // namespace neosycl::sycl
