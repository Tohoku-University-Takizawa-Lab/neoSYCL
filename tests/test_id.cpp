#include "gtest/gtest.h"
#include "CL/sycl.hpp"

using namespace cl::sycl;

TEST(id, basic_test) {
  id<1> index_1d(1);
  id<2> index_2d(1, 2);
  id<3> index_3d(1, 2, 3);

  EXPECT_EQ(index_1d[1], 1);
  EXPECT_EQ(index_1d.get(1), 1);

  EXPECT_EQ(index_2d[1], 1);
  EXPECT_EQ(index_2d[2], 2);
  EXPECT_EQ(index_2d.get(1), 1);
  EXPECT_EQ(index_2d.get(2), 2);

  EXPECT_EQ(index_3d[1], 1);
  EXPECT_EQ(index_3d[2], 2);
  EXPECT_EQ(index_3d[3], 3);
  EXPECT_EQ(index_3d.get(1), 1);
  EXPECT_EQ(index_3d.get(2), 2);
  EXPECT_EQ(index_3d.get(3), 3);

}

TEST(id, operator_test) {
  id<1> index_1d(1);
  id<1> index_1d_equal(1);
  id<1> index_1d_more(2);

  EXPECT_TRUE(index_1d < index_1d_more);
  EXPECT_TRUE(index_1d_more > index_1d);
  EXPECT_TRUE(index_1d != index_1d_more);
  EXPECT_TRUE(index_1d == index_1d_equal);

  id<2> index_2d(1, 2);
  id<2> index_2d_equal(1, 2);
  id<2> index_2d_more(2, 3);

  EXPECT_TRUE(index_2d < index_2d_more);
  EXPECT_TRUE(index_2d_more > index_2d);
  EXPECT_TRUE(index_2d != index_2d_more);
  EXPECT_TRUE(index_2d == index_2d_equal);

  id<3> index_3d(1, 2, 3);
  id<3> index_3d_equal(1, 2, 3);
  id<3> index_3d_more(2, 3, 4);

  EXPECT_TRUE(index_3d < index_3d_more);
  EXPECT_TRUE(index_3d_more > index_3d);
  EXPECT_TRUE(index_3d != index_3d_more);
  EXPECT_TRUE(index_3d == index_3d_equal);

}