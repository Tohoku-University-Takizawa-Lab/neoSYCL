#ifndef CUSTOM_SYCL_INCLUDE_SYCL_RANGE_H_
#define CUSTOM_SYCL_INCLUDE_SYCL_RANGE_H_

namespace cl::sycl {

template<std::size_t Dimensions = 1>
struct range {};

template<>
struct range<1> {
 private:
  size_t data[1];

 public:
  range(size_t x) : data{x} {}

  range(const range &obj) : data{obj.data[0]} {}

  range &operator=(const range &obj) {
    if (&obj != this) {
      data[0] = obj.data[0];
    }
    return *this;
  }

  size_t size() const {
    return data[0];
  }

  size_t get(int dimension) const {
    return data[dimension - 1];
  }

  size_t &operator[](int dimension) {
    return data[dimension - 1];
  }
};

template<>
struct range<2> {
 private:
  size_t data[2];

 public:
  range(size_t x, size_t y) : data{x, y} {}

  range(const range &obj) : data{obj.data[0], obj.data[1]} {}

  range &operator=(const range &obj) {
    if (&obj != this) {
      data[0] = obj.data[0];
      data[1] = obj.data[1];
    }
    return *this;
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
struct range<3> {
 private:
  size_t data[3];

 public:
  range(size_t x, size_t y, size_t z) : data{x, y, z} {}

  range(const range &obj) : data{obj.data[0], obj.data[1], obj.data[2]} {}

  range &operator=(const range &obj) {
    if (&obj != this) {
      data[0] = obj.data[0];
      data[1] = obj.data[1];
      data[2] = obj.data[2];
    }
    return *this;
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

#endif //CUSTOM_SYCL_INCLUDE_SYCL_RANGE_H_
