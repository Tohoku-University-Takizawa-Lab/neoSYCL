#ifndef SYCL_INCLUDE_CL_SYCL_ID_HPP_
#define SYCL_INCLUDE_CL_SYCL_ID_HPP_

#include "types.hpp"
#include "range.hpp"
#include "item.hpp"
#include "container/array_nd.hpp"

namespace neosycl::sycl {

template<std::size_t dimensions = 1>
struct id {};

template<>
struct id<1> {
 public:
  id() : data(0) {}

  id(size_t dim0) : data(dim0) {}

  id(const range<1> &range) : data(range.get(0)) {}

  id(const item<1> &item) : data(item.get_id(0)) {}

  size_t get(int dimension) const {
    return data[dimension];
  }

  size_t &operator[](int dimension) {
    return data[dimension];
  }

  size_t operator[](int dimension) const {
    return data[dimension];
  }

 private:
  detail::ArrayND<1> data;
};

template<>
struct id<2> {
 public:
  id() : data(0, 0) {}

  id(size_t dim0, size_t dim1) : data(dim0, dim1) {}

  id(const range<2> &range) : data(range.get(0), range.get(1)) {}

  id(const item<2> &item) : data(item.get_id(0), item.get_id(1)) {}

  size_t get(int dimension) const {
    return data[dimension];
  }

  size_t &operator[](int dimension) {
    return data[dimension];
  }

  size_t operator[](int dimension) const {
    return data[dimension];
  }

 private:
  detail::ArrayND<2> data;
};

template<>
struct id<3> {
 public:
  id() : data(0, 0, 0) {}

  id(size_t dim0, size_t dim1, size_t dim2) : data(dim0, dim1, dim2) {}

  id(const range<3> &range) :
      data(range.get(0), range.get(1), range.get(2)) {}

  id(const item<3> &item) :
      data(item.get_id(0), item.get_id(1), item.get_id(2)) {}

  size_t get(int dimension) const {
    return data[dimension];
  }

  size_t &operator[](int dimension) {
    return data[dimension];
  }

  size_t operator[](int dimension) const {
    return data[dimension];
  }

 private:
  detail::ArrayND<3> data;
};

}

#endif //SYCL_INCLUDE_CL_SYCL_ID_HPP_
