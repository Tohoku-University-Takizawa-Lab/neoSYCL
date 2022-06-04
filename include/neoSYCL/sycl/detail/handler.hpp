#pragma once

namespace neosycl::sycl::detail {

static void single_task(kernel& k, const std::function<void(void)>& func) {
  for (const auto& arg : k.get_acc()) {
    arg.acquire_access();
  }
  DEBUG_INFO("single_task(): %s\n", k.get_name());
  func();
  for (const auto& arg : k.get_acc()) {
    arg.release_access();
  }
}

static void parallel_for(kernel& k, range<1> r,
                         const std::function<void(id<1>)>& func, id<1> offset,
                         id<1>*) {
  for (const auto& arg : k.get_acc()) {
    arg.acquire_access();
  }
  DEBUG_INFO("parallel_for_1d(): %s\n", k.get_name());
  for (size_t x = offset.get(0); x < r.get(0); x++) {
    func(id<1>(x));
  }
  for (const auto& arg : k.get_acc()) {
    arg.release_access();
  }
}

static void parallel_for(kernel& k, range<1> r,
                         const std::function<void(item<1>)>& func, id<1> offset,
                         item<1>*) {
  for (const auto& arg : k.get_acc()) {
    arg.acquire_access();
  }
  DEBUG_INFO("parallel_for_1d(): %s\n", k.get_name());
  for (size_t x = offset.get(0); x < r.get(0); x++) {
    func(item<1>(r, {x}, {offset[0]}));
  }
  for (const auto& arg : k.get_acc()) {
    arg.release_access();
  }
}

static void parallel_for(kernel& k, range<2> r,
                         const std::function<void(id<2>)>& func, id<2> offset,
                         id<2>*) {
  for (const auto& arg : k.get_acc()) {
    arg.acquire_access();
  }
  DEBUG_INFO("parallel_for_2d(): %s\n", k.get_name());
  for (size_t y = offset.get(1); y < r.get(1); y++) {
    for (size_t x = offset.get(0); x < r.get(0); x++) {
      func(id<2>(x, y));
    }
  }
  for (const auto& arg : k.get_acc()) {
    arg.release_access();
  }
}

static void parallel_for(kernel& k, range<2> r,
                         const std::function<void(item<2>)>& func, id<2> offset,
                         item<2>*) {
  for (const auto& arg : k.get_acc()) {
    arg.acquire_access();
  }
  DEBUG_INFO("parallel_for_2d(): %s\n", k.get_name());
  for (size_t y = offset.get(1); y < r.get(1); y++) {
    for (size_t x = offset.get(0); x < r.get(0); x++) {
      func(item<2>(r, {x, y}, {offset[0], offset[1]}));
    }
  }
  for (const auto& arg : k.get_acc()) {
    arg.release_access();
  }
}

static void parallel_for(kernel& k, range<3> r,
                         const std::function<void(id<3>)>& func, id<3> offset,
                         id<3>*) {
  for (const auto& arg : k.get_acc()) {
    arg.acquire_access();
  }
  DEBUG_INFO("parallel_for_3d(): %s\n", k.get_name());
  for (size_t z = offset.get(2); z < r.get(2); z++) {
    for (size_t y = offset.get(1); y < r.get(1); y++) {
      for (size_t x = offset.get(0); x < r.get(0); x++) {
        func(id<3>(x, y, z));
      }
    }
  }
  for (const auto& arg : k.get_acc()) {
    arg.release_access();
  }
}

static void parallel_for(kernel& k, range<3> r,
                         const std::function<void(item<3>)>& func, id<3> offset,
                         item<3>*) {
  for (const auto& arg : k.get_acc()) {
    arg.acquire_access();
  }
  DEBUG_INFO("parallel_for_3d(): %s\n", k.get_name());
  for (size_t z = offset.get(2); z < r.get(2); z++) {
    for (size_t y = offset.get(1); y < r.get(1); y++) {
      for (size_t x = offset.get(0); x < r.get(0); x++) {
        func(item<3>(r, {x, y, z}, {offset[0], offset[1], offset[2]}));
      }
    }
  }
  for (const auto& arg : k.get_acc()) {
    arg.release_access();
  }
}
} // namespace neosycl::sycl::detail
