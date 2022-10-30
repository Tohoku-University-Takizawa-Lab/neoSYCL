#pragma once
#include <vector>
#include <future>
#include <unordered_set>

namespace neosycl::sycl {
namespace detail {
static size_t unique_id = 0;

template<typename T>
struct shared_future_class {
  shared_future_class(std::promise<T>& p){
    id = unique_id;
    unique_id++;
    ft = p.get_future().share();
  }

  T get() const {
    return ft.get();
  }

  void wait() const {
    return ft.wait();
  }

  template <class Rep, class Period>
  std::future_status
  wait_for(const std::chrono::duration<Rep, Period>& rel_time) const {
    return ft.wait_for(rel_time);
  }

  template <class Clock, class Duration>
  std::future_status
  wait_until(const std::chrono::time_point<Clock, Duration>& abs_time) const {
    return ft.wait_until(abs_time);
  }

  size_t id;
private:
  std::shared_future<T> ft;
};


struct Futures {
  template <access::mode mode>
  std::vector<shared_future_class<size_t>> get_futures() {
    if (mode == access::mode::read) {
      return writef;
    }
    else if (mode == access::mode::write || mode == access::mode::read_write) {
      auto ret = writef;
      ret.insert(ret.end(), readf.begin(), readf.end());
      return ret;
    }
    else {
      std::cout << "[Warning] access::mode::discard_* or access::mode::atomic "
                   "is not supported"
                << std::endl;
      return readf;
    }
  }

  void refresh(shared_future_class<size_t>& ft, access::mode mode) {
    if (mode == access::mode::read) {
      readf.push_back(ft);
    }
    else if (mode == access::mode::write || mode == access::mode::read_write) {
      writef.clear();
      readf.clear();
      writef.push_back(ft);
    }
    else {
      std::cout << "[Warning] access::mode::discard_* or access::mode::atomic "
                   "is not supported"
                << std::endl;
    }
    return;
  }

private:
  std::vector<shared_future_class<size_t>> readf, writef;
};
} // namespace detail
} // namespace neosycl::sycl

namespace std{
    template<typename T>
    class hash<neosycl::sycl::detail::shared_future_class<T>>{
        public:
        T operator () ( const neosycl::sycl::detail::shared_future_class<T> &p ) const{ return p.id;}
    };
}