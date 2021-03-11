#ifndef SYCL_INCLUDE_CL_SYCL_QUEUE_QUEUE_HPP_
#define SYCL_INCLUDE_CL_SYCL_QUEUE_QUEUE_HPP_

#include <thread>
#include <atomic>
#include <condition_variable>
#include <mutex>

namespace neosycl::sycl::detail {

class task_counter {

private:
  std::atomic<int> counter;
  std::condition_variable cond;
  std::mutex lock;

public:

  task_counter() : counter(0) {}

  void incr() {
    counter++;
  }

  void decr() {
    std::unique_lock<std::mutex> ul{lock};
    if (--counter == 0) {
      ul.unlock();
      cond.notify_all();
    }
  }

  void wait() {
    std::unique_lock<std::mutex> ul{lock};
    cond.wait(ul, [&] { return counter == 0; });
  }

};

}

#endif //SYCL_INCLUDE_CL_SYCL_QUEUE_QUEUE_HPP_
