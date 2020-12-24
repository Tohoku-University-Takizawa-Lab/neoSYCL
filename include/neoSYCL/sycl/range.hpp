#ifndef CUSTOM_SYCL_INCLUDE_SYCL_RANGE_H_
#define CUSTOM_SYCL_INCLUDE_SYCL_RANGE_H_

namespace neosycl::sycl {

template<std::size_t Dimensions = 1>
struct range {};

template<>
struct range<1> {
 public:
  /* The following constructor is only available in the range class specialization where: dimensions==1 */
  range(size_t dim0);

  size_t get(int dimension) const;

  size_t &operator[](int dimension);

  size_t operator[](int dimension) const;

  size_t size() const;

 private:
  size_t data[1];
};

template<>
struct range<2> {
 public:
  /* The following constructor is only available in the range class specialization where:
  dimensions==2 */
  range(size_t dim0, size_t dim1);

  size_t get(int dimension) const;

  size_t &operator[](int dimension);

  size_t operator[](int dimension) const;

  size_t size() const;

 private:
  size_t data[2];
};

template<>
struct range<3> {
 private:
  size_t data[3];

 public:
  /* The following constructor is only available in the range class specialization where:
  dimensions==3 */
  range(size_t dim0, size_t dim1, size_t dim2); /* -- common interface members -- */

  size_t get(int dimension) const;

  size_t &operator[](int dimension);

  size_t operator[](int dimension) const;

  size_t size() const;
};

range<1>::range(size_t dim0) : data{dim0} {}

size_t &range<1>::operator[](int dimension) {
  return data[dimension - 1];
}

size_t range<1>::operator[](int dimension) const {
  return data[dimension - 1];
}

size_t range<1>::size() const {
  return data[0];
}

size_t range<1>::get(int dimension) const {
  return data[dimension - 1];
}

range<2>::range(size_t x, size_t y) : data{x, y} {}

size_t range<2>::size() const {
  return data[0] * data[1];
}

size_t range<2>::get(int dimension) const {
  return data[dimension - 1];
}

size_t &range<2>::operator[](int dimension) {
  return data[dimension - 1];
}

size_t range<2>::operator[](int dimension) const {
  return data[dimension - 1];
}

range<3>::range(size_t dim0, size_t dim1, size_t dim2) : data{dim0, dim1, dim2} {}

size_t range<3>::size() const {
  return data[0] * data[1] * data[2];
}

size_t range<3>::get(int dimension) const {
  return data[dimension - 1];
}

size_t &range<3>::operator[](int dimension) {
  return data[dimension - 1];
}

size_t range<3>::operator[](int dimension) const {
  return data[dimension - 1];
}

}

#endif //CUSTOM_SYCL_INCLUDE_SYCL_RANGE_H_
