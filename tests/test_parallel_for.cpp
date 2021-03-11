#include "gtest/gtest.h"
#include "CL/sycl.hpp"

using namespace cl::sycl;

TEST(parallel_for, simple_test) {
  constexpr size_t N = 3;
  using Vector = float[N];

  Vector a = {1, 2, 3};
  Vector b = {5, 6, 8};
  Vector c;

  queue q;

  buffer<float> A{std::begin(a), std::end(a)};
  buffer<float> B{std::begin(b), std::end(b)};

  {
    buffer<float> C{c, N};

    q.submit([&](handler &cgh) {
      // In the kernel A and B are read, but C is written
      auto ka = A.get_access<access::mode::read>(cgh);
      auto kb = B.get_access<access::mode::read>(cgh);
      auto kc = C.get_access<access::mode::write>(cgh);

      // Enqueue a parallel kernel
      cgh.parallel_for<class vector_add>(range<1>(N), [=](id<1> index) {
        kc[index] = ka[index] + kb[index];
      });
    });
  }

  q.wait();

  std::cout << std::endl << "Result:" << std::endl;
  for (auto e : c)
    std::cout << e << " ";
  std::cout << std::endl;

  for (size_t i = 0; i < N; i++) {
    EXPECT_EQ(c[i], a[i] + b[i]);
  }
}