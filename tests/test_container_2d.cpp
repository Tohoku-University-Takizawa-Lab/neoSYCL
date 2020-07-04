#include "gtest/gtest.h"
#include "CL/sycl.hpp"

using namespace cl::sycl;

TEST(data_container_2d, create_from_ptr) {
  const int X = 10;
  const int Y = 5;
  int arr[X * Y];

  for (int i = 0; i < X * Y; i++) {
    arr[i] = i % Y;
  }
  detail::DataContainerND<int, 2> container(arr, range<2>(X, Y));

  for (int x = 0; x < X; x++) {
    for (int y = 0; y < Y; y++) {
      EXPECT_EQ(container[x][y], y);
      EXPECT_EQ(container.get(x, y), y);
    }
  }
}

TEST(data_container_2d, data_allocate) {
  const int X = 10;
  const int Y = 5;

  detail::DataContainerND<int, 2> container((range<2>(X, Y)));

  for (int x = 0; x < X; x++) {
    for (int y = 0; y < Y; y++) {
      container[x][y] = x + y;
    }
  }

  for (int x = 0; x < X; x++) {
    for (int y = 0; y < Y; y++) {
      EXPECT_EQ(container[x][y], x + y);
    }
  }
}

TEST(data_container_2d, copy_allocate) {
  const int X = 10;
  const int Y = 5;

  detail::DataContainerND<int, 2> container((range<2>(X, Y)));
  detail::DataContainerND<int, 2> container_cpy = container;

  for (int x = 0; x < X; x++) {
    for (int y = 0; y < Y; y++) {
      container[x][y] = x + y;
    }
  }

  for (int x = 0; x < X; x++) {
    for (int y = 0; y < Y; y++) {
      container_cpy[x][y] = 0;
    }
  }

  for (int x = 0; x < X; x++) {
    for (int y = 0; y < Y; y++) {
      EXPECT_EQ(container[x][y], x + y);
      EXPECT_EQ(container_cpy[x][y], 0);
    }
  }

}
