#ifndef SYCL_INCLUDE_CL_SYCL_ID_HPP_
#define SYCL_INCLUDE_CL_SYCL_ID_HPP_

#include "CL/SYCL/types.hpp"

namespace cl::sycl {

template<std::size_t Dimensions = 1>
struct id {};

template<>
struct id<1> {
 private:
  size_t data[1];

  double weight() const {
    return (double) data[0];
  }

 public:
  id() : data{0} {}

  id(size_t x) : data{x} {}

  id(const id &obj) : data{obj.data[0]} {}

  id &operator=(const id &obj) {
    if (&obj != this) {
      data[0] = obj.data[0];
    }
    return *this;
  }

  bool operator<(const id<1> &rhs) const {
    return weight() < rhs.weight();
  }
  bool operator>(const id<1> &rhs) const {
    return rhs.weight() < weight();
  }
  bool operator<=(const id<1> &rhs) const {
    return weight() <= rhs.weight();
  }
  bool operator>=(const id<1> &rhs) const {
    return rhs.weight() <= weight();
  }
  bool operator==(const id<1> &rhs) const {
    return rhs.weight() == weight();
  }
  bool operator!=(const id<1> &rhs) const {
    return rhs.weight() != weight();
  }

  size_t get(int dimension) const {
    return data[dimension - 1];
  }

  size_t &operator[](int dimension) {
    return data[dimension - 1];
  }
};

template<>
struct id<2> {
 private:
  size_t data[2];

  double weight() const {
    double weight_0 = data[0];
    double weight_1 = data[1];

    while (weight_0 > 10) {
      weight_0 /= (double) 10;
    }

    while (weight_1 > 1) {
      weight_1 /= (double) 10;
    }

    return weight_0 + weight_1;
  }

 public:
  id() : data{0, 0} {}

  id(size_t x, size_t y) : data{x, y} {}

  id(const id &obj) : data{obj.data[0], obj.data[1]} {}

  id &operator=(const id &obj) {
    if (&obj != this) {
      data[0] = obj.data[0];
      data[1] = obj.data[1];
    }
    return *this;
  }

  bool operator<(const id<2> &rhs) const {
    return weight() < rhs.weight();
  }
  bool operator>(const id<2> &rhs) const {
    return rhs.weight() < weight();
  }
  bool operator<=(const id<2> &rhs) const {
    return weight() <= rhs.weight();
  }
  bool operator>=(const id<2> &rhs) const {
    return rhs.weight() <= weight();
  }
  bool operator==(const id<2> &rhs) const {
    return rhs.weight() == weight();
  }
  bool operator!=(const id<2> &rhs) const {
    return rhs.weight() != weight();
  }

  size_t size() const {
    return data[0] * data[1];
  }

  size_t get(int dimension) const {
    return data[dimension - 1];
  }

  size_t &operator[](int dimension) {
    return data[dimension - 1];
  }
};

template<>
struct id<3> {
 private:
  size_t data[3];

  double weight() const {
    double weight_0 = data[0];
    double weight_1 = data[1];
    double weight_2 = data[2];

    while (weight_0 > 100) {
      weight_0 /= (double) 10;
    }

    while (weight_1 > 10) {
      weight_1 /= (double) 10;
    }

    while (weight_2 > 1) {
      weight_2 /= (double) 10;
    }

    return weight_0 + weight_1 + weight_2;
  }

 public:
  id() : data{0, 0, 0} {}

  id(size_t x, size_t y, size_t z) : data{x, y, z} {}

  id(const id &obj) : data{obj.data[0], obj.data[1], obj.data[2]} {}

  id &operator=(const id &obj) {
    if (&obj != this) {
      data[0] = obj.data[0];
      data[1] = obj.data[1];
      data[2] = obj.data[2];
    }
    return *this;
  }

  bool operator<(const id<3> &rhs) const {
    return weight() < rhs.weight();
  }
  bool operator>(const id<3> &rhs) const {
    return rhs.weight() < weight();
  }
  bool operator<=(const id<3> &rhs) const {
    return weight() <= rhs.weight();
  }
  bool operator>=(const id<3> &rhs) const {
    return rhs.weight() <= weight();
  }
  bool operator==(const id<3> &rhs) const {
    return rhs.weight() == weight();
  }
  bool operator!=(const id<3> &rhs) const {
    return rhs.weight() != weight();
  }

  size_t size() const {
    return data[0] * data[1] * data[2];
  }

  size_t get(int dimension) const {
    return data[dimension - 1];
  }

  size_t &operator[](int dimension) {
    return data[dimension - 1];
  }

};

}

#endif //SYCL_INCLUDE_CL_SYCL_ID_HPP_
