#ifndef SYCL_INCLUDE_CL_SYCL_QUEUE_QUEUE_HPP_
#define SYCL_INCLUDE_CL_SYCL_QUEUE_QUEUE_HPP_

#include <thread>
#include <atomic>
#include <condition_variable>
#include <mutex>

namespace cl::sycl::detail {

class TaskQueue {

 private:
  std::atomic<int> counter;
  std::condition_variable cond;
  std::mutex queue_mutex;

 public:

  TaskQueue() : counter(0) {}

  void thread_start() {
#ifndef SYNC
    counter++;
    DEBUG_INFO("[TaskQueue] thread start ,current running: {}", counter);
#endif
  }

  void thread_end() {
#ifndef SYNC
    {
      std::unique_lock<std::mutex> ul{queue_mutex};
      if (--counter == 0) {
        ul.unlock();
        cond.notify_all();
      }
    }
    DEBUG_INFO("[TaskQueue] thread end ,current running: {}", counter);
#endif
  }

  void wait() {
#ifndef SYNC
    DEBUG_INFO("[TaskQueue] wait for tasks, current running: {}", counter);
    {
      std::unique_lock<std::mutex> ul{queue_mutex};
      cond.wait(ul, [&] { return counter == 0; });
    }
#endif
  }

};

}

#endif //SYCL_INCLUDE_CL_SYCL_QUEUE_QUEUE_HPP_
