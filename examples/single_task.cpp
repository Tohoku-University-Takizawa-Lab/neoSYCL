#include <CL/sycl.hpp>
#include <iostream>
#include <thread>
#include <unistd.h>

using namespace cl::sycl;

int main() {
  long arr[] = {1, 2, 3};

  {
    std::shared_ptr<long[]> ptr(arr);

    std::function<void(void)> func = [=]() {
      sleep(2);
      std::cout << "output: " << ptr[2] << std::endl;
      ptr[2] = 233;
    };

    std::thread t(func);
    t.detach();
  }

  sleep(4);

  std::cout << "end" << std::endl;

}
