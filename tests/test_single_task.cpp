#include "gtest/gtest.h"
#include "CL/sycl.hpp"

using namespace cl::sycl;

TEST(queue, single_task) {
  queue q;
  size_t SIZE = 10;
  float val[10];
  buffer<float, 1> buf(val, range<1>(SIZE));

  q.submit([&](handler& cgh) {
    auto acc = buf.get_access<access::mode::read_write>(cgh);
    cgh.single_task<class simple_test>([=]() { acc[0] = 233; });
  });
  q.wait();

  EXPECT_EQ(val[0], 233);
}

TEST(queue, data_consistency) {
  queue q;
  size_t SIZE = 1;
  float val[1]{0};
  buffer<float, 1> buf(val, range<1>(SIZE));

  for (int i = 0; i < 10; i++) {
    q.submit([&](handler& cgh) {
      auto acc = buf.get_access<access::mode::read_write>(cgh);
      cgh.single_task<class simple_test>([=]() { acc[0] += 1; });
    });
  }

  q.wait();

  EXPECT_EQ(val[0], 10);
}