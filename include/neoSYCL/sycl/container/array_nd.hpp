#ifndef NEOSYCL_INCLUDE_NEOSYCL_SYCL_BUFFER_ARRAY_ND_HPP_
#define NEOSYCL_INCLUDE_NEOSYCL_SYCL_BUFFER_ARRAY_ND_HPP_

namespace neosycl::sycl::detail {

template<size_t dimensions = 1>
struct ArrayND {
  ArrayND() : data{} {}

  template<int D = dimensions, typename = std::enable_if_t<D == 1>>
  ArrayND(size_t dim0) : data{dim0} {}

  template<int D = dimensions, typename = std::enable_if_t<D == 2>>
  ArrayND(size_t dim0, size_t dim1) : data{dim0, dim1} {}

  template<int D = dimensions, typename = std::enable_if_t<D == 3>>
  ArrayND(size_t dim0, size_t dim1, size_t dim2) : data{dim0, dim1, dim2} {}

  size_t operator[](int dimension) const {
    return data[dimension];
  }

  size_t &operator[](int dimension) {
    return data[dimension];
  }

  size_t get_liner() const {
    if (dimensions == 2) {
      return data[0] * data[1];
    } else if (dimensions == 3) {
      return data[0] * data[1] * data[2];
    }
    return data[0];
  }

 private:
  size_t data[dimensions];
};

}

#endif //NEOSYCL_INCLUDE_NEOSYCL_SYCL_BUFFER_ARRAY_ND_HPP_
