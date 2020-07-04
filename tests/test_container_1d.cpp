#include "gtest/gtest.h"
#include "CL/sycl.hpp"

using namespace cl::sycl;

TEST(data_container_1d, create_from_ptr) {
  const int ARRAY_SIZE = 100;
  int arr[ARRAY_SIZE];
  for (int i = 0; i < ARRAY_SIZE; i++) {
    arr[i] = i;
  }
  detail::DataContainerND<int, 1> container(arr, range<1>(ARRAY_SIZE));

  EXPECT_EQ(container.get_size(), sizeof(int) * ARRAY_SIZE);

  for (int i = 0; i < ARRAY_SIZE; i++) {
    EXPECT_EQ(container[i], i);
    EXPECT_EQ(container.get(i), i);
  }
}

TEST(data_container_1d, data_allocate) {
  const int ARRAY_SIZE = 100;

  detail::DataContainerND<int, 1> container((range<1>(ARRAY_SIZE)));

  for (int i = 0; i < ARRAY_SIZE; i++) {
    container[i] = i;
  }

  for (int i = 0; i < ARRAY_SIZE; i++) {
    EXPECT_EQ(container[i], i);
    EXPECT_EQ(container.get(i), i);
  }
}

TEST(data_container_1d, copy_allocate) {
  const int ARRAY_SIZE = 100;

  detail::DataContainerND<int, 1> container((range<1>(ARRAY_SIZE)));
  detail::DataContainerND<int, 1> container_cpy = container;

  for (int i = 0; i < ARRAY_SIZE; i++) {
    container[i] = i;
  }

  for (int i = 0; i < ARRAY_SIZE; i++) {
    container_cpy[i] = 0;
  }

  for (int i = 0; i < ARRAY_SIZE; i++) {
    EXPECT_EQ(container[i], i);
    EXPECT_EQ(container_cpy[i], 0);
  }
}

TEST(data_container_1d, copy_from_raw) {
  const int ARRAY_SIZE = 100;
  int arr[ARRAY_SIZE];
  for (int i = 0; i < ARRAY_SIZE; i++) {
    arr[i] = i;
  }

  detail::DataContainerND<int, 1> container(arr, range<1>(ARRAY_SIZE));
  detail::DataContainerND<int, 1> container_cpy = container;

  for (int i = 0; i < ARRAY_SIZE; i++) {
    container[i] = i;
  }

  for (int i = 0; i < ARRAY_SIZE; i++) {
    container_cpy[i] = 0;
  }

  for (int i = 0; i < ARRAY_SIZE; i++) {
    EXPECT_EQ(container[i], 0);
    EXPECT_EQ(container_cpy[i], 0);
  }
}