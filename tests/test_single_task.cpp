#include "gtest/gtest.h"
#include "CL/sycl.hpp"

using namespace cl::sycl;

TEST(queue, single_task) {
  queue myQueue;
  myQueue.submit([&](handler &cgh) {
    cgh.single_task<class simple_test>([=]() {
      std::cout << "single task" << std::endl;
    });
  });
  myQueue.wait();
}