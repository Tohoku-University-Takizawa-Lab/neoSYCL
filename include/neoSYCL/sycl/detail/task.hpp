#ifndef NEOSYCL_INCLUDE_NEOSYCL_SYCL_DETAIL_TASK_HPP
#define NEOSYCL_INCLUDE_NEOSYCL_SYCL_DETAIL_TASK_HPP

namespace neosycl::sycl::detail {

struct task {
private:
  bool waiting;
  std::condition_variable cond;
  std::mutex lock;

  void start() {
    waiting = true;
  }

  void end() {
    std::unique_lock<std::mutex> ul{lock};
    waiting = false;
    ul.unlock();
    cond.notify_all();
  }

  void wait() {
    std::unique_lock<std::mutex> ul{lock};
    cond.wait(ul, [&] { return !waiting; });
  }

};

}

#endif //NEOSYCL_INCLUDE_NEOSYCL_SYCL_DETAIL_TASK_HPP
