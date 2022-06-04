#pragma once

namespace neosycl::sycl::rt {

using namespace neosycl::sycl;

template <typename T>
struct acc_ {
  T* p_;
  size_t r_[6];

  inline T& operator[](size_t i) {
    return p_[i];
  }
  inline T& operator[](const id<3> i) {
    return p_[(i[2] * r_[1] + i[1]) * r_[0] + i[0]];
  }
  inline T& operator[](const id<2> i) {
    return p_[i[1] * r_[0] + i[0]];
  }
  inline T& operator[](const id<1> i) {
    return p_[i[0]];
  }

  template <size_t dimensions>
  T& operator[](item<dimensions>& i) {
    return this->operator[](i.get_id());
  }
};

inline item<1> id2item(size_t s[6], size_t i0) {
  return item<1>(range<1>{s[0]}, detail::container::ArrayND<1>{i0},
                 detail::container::ArrayND<1>{s[3]});
}

inline item<2> id2item(size_t s[6], size_t i0, size_t i1) {
  return item<2>(range<2>{s[0], s[1]}, detail::container::ArrayND<2>{i0, i1},
                 detail::container::ArrayND<2>{s[3], s[4]});
}

inline item<3> id2item(size_t s[6], size_t i0, size_t i1, size_t i2) {
  return item<3>(range<3>{s[0], s[1], s[2]},
                 detail::container::ArrayND<3>{i0, i1, i2},
                 detail::container::ArrayND<3>{s[3], s[4], s[5]});
}

} // namespace neosycl::sycl::rt
