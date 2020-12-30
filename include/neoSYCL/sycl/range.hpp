#ifndef CUSTOM_SYCL_INCLUDE_SYCL_RANGE_H_
#define CUSTOM_SYCL_INCLUDE_SYCL_RANGE_H_

#include "container/array_nd.hpp"

namespace neosycl::sycl {

template<std::size_t Dimensions = 1>
struct range {};

template<>
struct range<1> {
 public:
  /* The following constructor is only available in the range class specialization where: dimensions==1 */
  range(size_t dim0) : data(dim0) {}

  size_t get(int dimension) const {
    return data[dimension];
  }

  size_t &operator[](int dimension) {
    return data[dimension];
  }

  size_t operator[](int dimension) const {
    return data[dimension];
  }

  size_t size() const {
    return data.get_liner();
  }

 private:
  detail::ArrayND<1> data;
};

template<>
struct range<2> {
 public:
  /* The following constructor is only available in the range class specialization where:
  dimensions==2 */
  range(size_t dim0, size_t dim1) : data{dim0, dim1} {}

  size_t get(int dimension) const {
    return data[dimension];
  }

  size_t &operator[](int dimension) {
    return data[dimension];
  }

  size_t operator[](int dimension) const {
    return data[dimension];
  }

  size_t size() const {
    return data.get_liner();
  }

 private:
  detail::ArrayND<2> data;
};

template<>
struct range<3> {
 public:
  /* The following constructor is only available in the range class specialization where:
  dimensions==3 */
  range(size_t dim0, size_t dim1, size_t dim2) : data(dim0, dim1, dim2) {}

  size_t get(int dimension) const {
    return data[dimension];
  }

  size_t &operator[](int dimension) {
    return data[dimension];
  }

  size_t operator[](int dimension) const {
    return data[dimension];
  }

  size_t size() const {
    return data.get_liner();
  }

 private:
  detail::ArrayND<3> data;
};

}

#endif //CUSTOM_SYCL_INCLUDE_SYCL_RANGE_H_
