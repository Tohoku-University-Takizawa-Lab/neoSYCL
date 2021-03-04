#ifndef CUSTOM_SYCL_INCLUDE_SYCL_RANGE_H_
#define CUSTOM_SYCL_INCLUDE_SYCL_RANGE_H_

#include "detail/container/array_nd.hpp"

namespace neosycl::sycl {

template<std::size_t dimensions = 1>
struct range {
public:
  template<int D = dimensions, typename = std::enable_if_t<D == 1>>
  range(size_t dim0) :data(dim0) {}

  template<int D = dimensions, typename = std::enable_if_t<D == 2>>
  range(size_t dim0, size_t dim1) : data(dim0, dim1) {}

  template<int D = dimensions, typename = std::enable_if_t<D == 3>>
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
  detail::container::ArrayND<dimensions> data;
};

}

#endif //CUSTOM_SYCL_INCLUDE_SYCL_RANGE_H_
