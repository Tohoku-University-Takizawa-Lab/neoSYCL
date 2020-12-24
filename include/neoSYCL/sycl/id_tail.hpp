#ifndef NEOSYCL_INCLUDE_NEOSYCL_SYCL_ID_TAIL_HPP_
#define NEOSYCL_INCLUDE_NEOSYCL_SYCL_ID_TAIL_HPP_

#include "range.hpp"
#include "item.hpp"

namespace neosycl::sycl {

id<1>::id() : data{0} {}

id<1>::id(size_t dim0) : data{dim0} {}

id<1>::id(const range<1> &range) : data{range.get(1)} {}

id<1>::id(const item<1> &ite) : data{ite.(1)} {}

double id<1>::weight() const {
  return (double) data[0];
}

size_t id<1>::operator[](int dimension) const {
  return data[dimension - 1];
}

bool id<1>::operator>(const id<1> &rhs) const {
  return rhs.weight() > weight();
}

bool id<1>::operator<(const id<1> &rhs) const {
  return rhs.weight() < weight();
}

bool id<1>::operator<=(const id<1> &rhs) const {
  return weight() <= rhs.weight();
}
bool id<1>::operator>=(const id<1> &rhs) const {
  return rhs.weight() <= weight();
}
bool id<1>::operator==(const id<1> &rhs) const {
  return rhs.weight() == weight();
}
bool id<1>::operator!=(const id<1> &rhs) const {
  return rhs.weight() != weight();
}

double id<2>::weight() const {
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

id<2>::id() : data{0, 0} {}

id<2>::id(const range<2> &range) : data{range[1], range[2]} {}

id<2>::id(const item<2> &item) : data{item.get(1)} {}

size_t id<2>::operator[](int dimension) const {
  return data[dimension - 1];
}

bool id<2>::operator>(const id<2> &rhs) const {
  return rhs.weight() > weight();
}

bool id<2>::operator<(const id<2> &rhs) const {
  return rhs.weight() < weight();
}

bool id<2>::operator<=(const id<2> &rhs) const {
  return weight() <= rhs.weight();
}
bool id<2>::operator>=(const id<2> &rhs) const {
  return rhs.weight() <= weight();
}
bool id<2>::operator==(const id<2> &rhs) const {
  return rhs.weight() == weight();
}
bool id<2>::operator!=(const id<2> &rhs) const {
  return rhs.weight() != weight();
}

id<3>::id() : data{0, 0, 0} {}

id<3>::id(const range<3> &range) : data{range[1], range[2]} {}

id<3>::id(const item<3> &item) : data{item.get(1)} {}

size_t id<3>::operator[](int dimension) const {
  return data[dimension - 1];
}

bool id<3>::operator>(const id<3> &rhs) const {
  return rhs.weight() > weight();
}

bool id<3>::operator<(const id<3> &rhs) const {
  return rhs.weight() < weight();
}

bool id<3>::operator<=(const id<3> &rhs) const {
  return weight() <= rhs.weight();
}
bool id<3>::operator>=(const id<3> &rhs) const {
  return rhs.weight() <= weight();
}
bool id<3>::operator==(const id<3> &rhs) const {
  return rhs.weight() == weight();
}
bool id<3>::operator!=(const id<3> &rhs) const {
  return rhs.weight() != weight();
}

double id<3>::weight() const {
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

}

#endif //NEOSYCL_INCLUDE_NEOSYCL_SYCL_ID_TAIL_HPP_
