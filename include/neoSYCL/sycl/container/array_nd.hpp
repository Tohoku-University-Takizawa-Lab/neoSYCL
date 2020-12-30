#ifndef NEOSYCL_INCLUDE_NEOSYCL_SYCL_BUFFER_ARRAY_ND_HPP_
#define NEOSYCL_INCLUDE_NEOSYCL_SYCL_BUFFER_ARRAY_ND_HPP_

namespace neosycl::sycl::detail {

template<size_t Dimensions = 1>
struct ArrayND {};

template<>
struct ArrayND<1> {
 private:
  size_t data[1];

 public:
  ArrayND(size_t dim0) : data{dim0} {}

  size_t &operator[](int dimension) {
    assert(dimensions == 0);
    return data[0];
  }

  size_t operator[](int dimension) const {
    assert(dimensions == 0);
    return data[0];
  }

  size_t get_liner() const {
    return data[0];
  }

};

template<>
struct ArrayND<2> {
 private:
  size_t data[2];

 public:
  ArrayND(size_t dim0, size_t dim1) : data{dim0, dim1} {}

  size_t &operator[](int dimension) {
    assert(0 <= dimensions <= 1);
    return data[dimension];
  }

  size_t operator[](int dimension) const {
    assert(0 <= dimensions <= 1);
    return data[dimension];
  }

  size_t get_liner() const {
    return data[0] * data[1];
  }
};

template<>
struct ArrayND<3> {
 private:
  size_t data[3];

 public:
  ArrayND(size_t dim0, size_t dim1, size_t dim2) : data{dim0, dim1, dim2} {}

  size_t &operator[](int dimension) {
    assert(0 <= dimensions <= 2);
    return data[dimension];
  }

  size_t operator[](int dimension) const {
    assert(0 <= dimensions <= 2);
    return data[dimension];
  }

  size_t get_liner() const {
    return data[0] * data[1] * data[2];
  }
};

}

#endif //NEOSYCL_INCLUDE_NEOSYCL_SYCL_BUFFER_ARRAY_ND_HPP_
