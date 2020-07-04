#include "gtest/gtest.h"
#include "CL/sycl.hpp"

using namespace cl::sycl;

TEST(data_container_3d, create_from_ptr) {
  const int X = 10;
  const int Y = 5;
  const int Z = 4;
  int arr[X * Y * Z];

  for (int i = 0; i < X * Y * Z; i++) {
    arr[i] = i % Z;
  }
  detail::DataContainerND<int, 3> container(arr, range<3>(X, Y, Z));

  for (int x = 0; x < X; x++) {
    for (int y = 0; y < Y; y++) {
      for (int z = 0; z < Z; z++) {
        EXPECT_EQ(container[x][y][z], z);
        EXPECT_EQ(container.get(x, y, z), z);
      }
    }
  }
}

TEST(data_container_3d, data_allocate) {
  const int X = 10;
  const int Y = 5;
  const int Z = 4;

  detail::DataContainerND<int, 3> container(range<3>(X, Y, Z));

  for (int x = 0; x < X; x++) {
    for (int y = 0; y < Y; y++) {
      for (int z = 0; z < Z; z++) {
        container[x][y][z] = y + z;
      }
    }
  }

  for (int x = 0; x < X; x++) {
    for (int y = 0; y < Y; y++) {
      for (int z = 0; z < Z; z++) {
        EXPECT_EQ(container[x][y][z], y + z);
        EXPECT_EQ(container.get(x, y, z), y + z);
      }
    }
  }
}