#include "gtest/gtest.h"
#include "CL/sycl.hpp"

using namespace cl::sycl;

TEST(data_container, simple_multi_thread) {
  const int ARRAY_SIZE = 1;
  detail::DataContainerND<int, 1> b((range<1>(ARRAY_SIZE)));

  b.lock_read();

  std::thread thread_a([&]() {
    b.lock_write();
    b[0] = 1234;
    b.unlock_write();
  });

  std::thread thread_b([&]() {
    b.lock_read();
    b[0] = 2345;
    b.unlock_read();
  });
  thread_b.join();

  b.unlock_read();

  thread_a.join();

  EXPECT_EQ(b[0], 1234);
}


