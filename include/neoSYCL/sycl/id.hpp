#ifndef SYCL_INCLUDE_CL_SYCL_ID_HPP_
#define SYCL_INCLUDE_CL_SYCL_ID_HPP_

#include "types.hpp"
#include "range.hpp"
#include "item.hpp"

namespace neosycl::sycl {


template<std::size_t dimensions = 1>
struct id {};

template<>
struct id<1> {
 public:
  id();

  id(size_t dim0);

  id(const range<1> &range);

  id(const item<1> &item);

  size_t get(int dimension) const;

  size_t &operator[](int dimension);

  size_t operator[](int dimension) const;

  bool operator<(const id<1> &rhs) const;

  bool operator>(const id<1> &rhs) const;

  bool operator<=(const id<1> &rhs) const;

  bool operator>=(const id<1> &rhs) const;

  bool operator==(const id<1> &rhs) const;

  bool operator!=(const id<1> &rhs) const;

 private:
  size_t data[1];

  double weight() const;

};

template<>
struct id<2> {
 public:
  id();

  id(size_t dim0);

  id(const range<2> &range);

  id(const item<2> &item);

  size_t get(int dimension) const;

  size_t &operator[](int dimension);

  size_t operator[](int dimension) const;

  bool operator<(const id<2> &rhs) const;

  bool operator>(const id<2> &rhs) const;

  bool operator<=(const id<2> &rhs) const;

  bool operator>=(const id<2> &rhs) const;

  bool operator==(const id<2> &rhs) const;

  bool operator!=(const id<2> &rhs) const;

 private:
  size_t data[2];

  double weight() const;
};

template<>
struct id<3> {
 public:
  id();

  id(size_t dim0);

  id(const range<3> &range);

  id(const item<3> &item);

  size_t get(int dimension) const;

  size_t &operator[](int dimension);

  size_t operator[](int dimension) const;

  bool operator<(const id<3> &rhs) const;

  bool operator>(const id<3> &rhs) const;

  bool operator<=(const id<3> &rhs) const;

  bool operator>=(const id<3> &rhs) const;

  bool operator==(const id<3> &rhs) const;

  bool operator!=(const id<3> &rhs) const;

 private:
  size_t data[3];

  double weight() const;
};

}

#endif //SYCL_INCLUDE_CL_SYCL_ID_HPP_
