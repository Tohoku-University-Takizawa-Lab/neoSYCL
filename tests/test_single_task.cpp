#include "gtest/gtest.h"
#include "CL/sycl.hpp"

using namespace cl::sycl;

TEST(sycl, basic_single_task) {
  int result; // this is where we will write our result

  range<2> z{1, 3};
  { // by sticking all the SYCL work in a {} block, we ensure
    // all SYCL tasks must complete before exiting the block

    //  create a queue to work on
    queue myQueue;
    buffer<int, 3> Buf(range<3>{1, 2, 3});

    // wrap our result variable in a buffer
    buffer<int> resultBuf(&result, 1);

    // create some "commands" for our "queue"
    myQueue.submit([&](handler &cgh) {
      // request access to our buffer
      auto writeResult = resultBuf.get_access<access::mode::write>(cgh);

      // enqueue a single, simple task
      cgh.single_task<class simple_test>([=]() {
        writeResult[0] = 1234;
      });
    }); // end of our commands for this queue

  } // end scope, so we wait for the queue to complete

  printf("Result = %d\n", result);

  EXPECT_EQ(result, 1234);
}

TEST(sycl, vector_add) {
  const int N = 3;
  using Vector = float[N];

  Vector a = {1, 2, 3};
  Vector b = {5, 6, 8};

  float c[N];

  { // By sticking all the SYCL work in a {} block, we ensure
    // all SYCL tasks must complete before exiting the block

    // Create a queue to work on
    queue myQueue;

    // Create buffers from a & b vectors with 2 different syntax
    buffer<float> A(std::begin(a), std::end(a));
    buffer<float> B{std::begin(b), std::end(b)};

    // A buffer of N float using the storage of c
    buffer<float> C(c, N);

    /* The command group describing all operations needed for the kernel
       execution */
    myQueue.submit([&](handler &cgh) {
      // In the kernel A and B are read, but C is written
      auto ka = A.get_access<access::mode::read>(cgh);
      auto kb = B.get_access<access::mode::read>(cgh);
      auto kc = C.get_access<access::mode::write>(cgh);

      // Enqueue a single, simple task
      cgh.single_task<class sequential_vector>([=]() {
        for (size_t i = 0; i != N; i++)
          kc[i] = ka[i] + kb[i];
      });
    }); // End of our commands for this queue
  } // End scope, so we wait for the queue to complete

  std::cout << "Result:" << std::endl;
  for (size_t i = 0; i != N; i++)
    std::cout << c[i] << " ";
  std::cout << std::endl;

  for (size_t i = 0; i < N; i++) {
    EXPECT_EQ(c[i], a[i] + b[i]);
  }
}