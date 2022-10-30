#pragma once
#include <vector>
#include <future>
namespace neosycl::sycl {

namespace detail {
struct Futures {
  template <access::mode mode>
  std::vector<std::shared_future<size_t>> get_futures() {
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

  void refresh(std::shared_future<size_t>& ft, access::mode mode) {
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
  std::vector<std::shared_future<size_t>> readf, writef;
};
} // namespace detail
} // namespace neosycl::sycl