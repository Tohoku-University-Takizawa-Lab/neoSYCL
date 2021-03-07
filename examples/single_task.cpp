#include <CL/sycl.hpp>
#include <iostream>
#include <unistd.h>

using namespace cl::sycl;


int main() {
  queue myQueue;
  myQueue.submit([&](handler &cgh) {
    cgh.single_task<class simple_test>([=]() {
      std::cout << "single task" << std::endl;
    });
  });
  myQueue.wait();
}
