#pragma once

namespace neosycl::sycl::detail {

inline void single_task(const std::function<void(void)>& func) {
  func();
}

inline void parallel_for(range<1> r, const std::function<void(id<1>)>& func,
                         id<1> offset, id<1>*) {
  for (size_t x = offset.get(0); x < r.get(0); x++) {
    func(id<1>(x));
  }
}

inline void parallel_for(range<1> r, const std::function<void(item<1>)>& func,
                         id<1> offset, item<1>*) {
  for (size_t x = offset.get(0); x < r.get(0); x++) {
    func(item<1>(r, {x}, {offset[0]}));
  }
}

inline void parallel_for(range<2> r, const std::function<void(id<2>)>& func,
                         id<2> offset, id<2>*) {
  for (size_t x = offset.get(0); x < r.get(0); x++) {
    for (size_t y = offset.get(1); y < r.get(1); y++) {
      func(id<2>(x, y));
    }
  }
}

inline void parallel_for(range<2> r, const std::function<void(item<2>)>& func,
                         id<2> offset, item<2>*) {
  for (size_t x = offset.get(0); x < r.get(0); x++) {
    for (size_t y = offset.get(1); y < r.get(1); y++) {
      func(item<2>(r, {x, y}, {offset[0], offset[1]}));
    }
  }
}

inline void parallel_for(range<3> r, const std::function<void(id<3>)>& func,
                         id<3> offset, id<3>*) {
  for (size_t x = offset.get(0); x < r.get(0); x++) {
    for (size_t y = offset.get(1); y < r.get(1); y++) {
      for (size_t z = offset.get(2); z < r.get(2); z++) {
        func(id<3>(x, y, z));
      }
    }
  }
}

inline void parallel_for(range<3> r, const std::function<void(item<3>)>& func,
                         id<3> offset, item<3>*) {
  for (size_t x = offset.get(0); x < r.get(0); x++) {
    for (size_t y = offset.get(1); y < r.get(1); y++) {
      for (size_t z = offset.get(2); z < r.get(2); z++) {
        func(item<3>(r, {x, y, z}, {offset[0], offset[1], offset[2]}));
      }
    }
  }
}
} // namespace neosycl::sycl::detail
