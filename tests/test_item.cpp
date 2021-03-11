#include "gtest/gtest.h"
#include "CL/sycl.hpp"

using namespace cl::sycl;

TEST(item, basic_test) {
  range<2> range_2d(1, 2);
  range<3> range_3d(1, 2, 3);

  id<1> id_1d(1);
  id<2> id_2d(1, 2);
  id<3> id_3d(1, 2, 3);

  EXPECT_EQ(id_1d[0], 1);
  EXPECT_EQ(id_1d.get(0), 1);

  EXPECT_EQ(id_2d[0], 1);
  EXPECT_EQ(id_2d[1], 2);
  EXPECT_EQ(id_3d.get(0), 1);
  EXPECT_EQ(id_3d.get(1), 2);
  EXPECT_EQ(id_3d.get(2), 3);

  EXPECT_EQ(id<1>(range<1>(1))[0], 1);
}

TEST(id, copy_test) {
  id<2> id_2d(1, 2);
  id<2> cpy = id_2d;

  EXPECT_EQ(id_2d[0], cpy[0]);
  EXPECT_EQ(id_2d[1], cpy[1]);
}