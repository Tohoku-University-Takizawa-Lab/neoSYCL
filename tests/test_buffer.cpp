#include "gtest/gtest.h"
#include "CL/sycl.hpp"

using namespace cl::sycl;

TEST(buffer, container_test) {

  detail::container::DataContainerND<int, 1> container(10);

  for (int i = 0; i < 10; i++) {
    container[i] = i;
  }

  for (int i = 0; i < 10; i++) {
    EXPECT_EQ(container[i], i);
  }

  detail::container::DataContainerND<int, 1> copy_container(container);

  for (int i = 0; i < 10; i++) {
    EXPECT_EQ(copy_container[i], i);
  }

  EXPECT_TRUE(container.get_ptr() != copy_container.get_ptr());
}

TEST(buffer, container_create_from_arr) {
  float val[10];

  for (int i = 0; i < 10; i++) {
    val[i] = float(i);
  }

  {
    detail::container::DataContainerND<float, 1> container(val, 10);

    for (int i = 0; i < 10; i++) {
      EXPECT_EQ(container[i], i);
      container[i] = i + 1;
    }
  }

  for (int i = 0; i < 10; i++) {
    EXPECT_EQ(val[i], i + 1);
  }
}

TEST(buffer, buffer_basic_tests) {
  size_t SIZE = 10;
  float val[10];

  buffer<float, 1> buf(val, range<1>(SIZE));

  EXPECT_EQ(buf.get_count(), SIZE);
  EXPECT_EQ(buf.get_size(), SIZE * sizeof(float));
  EXPECT_EQ(buf.get_range(), range<1>(SIZE));
}
