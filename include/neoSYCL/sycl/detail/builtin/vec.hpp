#pragma once
#include <type_traits>

namespace neosycl::sycl {

enum class rounding_mode { automatic, rte, rtz, rtp, rtn };

struct elem {
  static constexpr int x  = 0;
  static constexpr int y  = 1;
  static constexpr int z  = 2;
  static constexpr int w  = 3;
  static constexpr int r  = 0;
  static constexpr int g  = 1;
  static constexpr int b  = 2;
  static constexpr int a  = 3;
  static constexpr int s0 = 0;
  static constexpr int s1 = 1;
  static constexpr int s2 = 2;
  static constexpr int s3 = 3;
  static constexpr int s4 = 4;
  static constexpr int s5 = 5;
  static constexpr int s6 = 6;
  static constexpr int s7 = 7;
  static constexpr int s8 = 8;
  static constexpr int s9 = 9;
  static constexpr int sA = 10;
  static constexpr int sB = 11;
  static constexpr int sC = 12;
  static constexpr int sD = 13;
  static constexpr int sE = 14;
  static constexpr int sF = 15;
};

////////////////////////////////////////////////////////////////////////////////
template <typename DataT, size_t NumElems>
class vec {
  FRIEND_VEC_BINARY_OPERATOR(+);
  FRIEND_VEC_BINARY_OPERATOR(-);
  FRIEND_VEC_BINARY_OPERATOR(*);
  FRIEND_VEC_BINARY_OPERATOR(/);
  FRIEND_VEC_BINARY_OPERATOR(%);
  FRIEND_VEC_ASSIGN_OPERATOR(+=);
  FRIEND_VEC_ASSIGN_OPERATOR(-=);
  FRIEND_VEC_ASSIGN_OPERATOR(*=);
  FRIEND_VEC_ASSIGN_OPERATOR(/=);
  FRIEND_VEC_ASSIGN_OPERATOR(%=);
  FRIEND_VEC_INCDEC_OPERATOR(++);
  FRIEND_VEC_INCDEC_OPERATOR(--);
  FRIEND_VEC_BINARY_OPERATOR(&);
  FRIEND_VEC_BINARY_OPERATOR(|);
  FRIEND_VEC_BINARY_OPERATOR(^);
  FRIEND_VEC_ASSIGN_OPERATOR(&=);
  FRIEND_VEC_ASSIGN_OPERATOR(|=);
  FRIEND_VEC_ASSIGN_OPERATOR(^=);
  FRIEND_VEC_BOOLEAN_OPERATOR(&&);
  FRIEND_VEC_BOOLEAN_OPERATOR(||);
  FRIEND_VEC_BINARY_OPERATOR(<<);
  FRIEND_VEC_BINARY_OPERATOR(>>);
  FRIEND_VEC_ASSIGN_OPERATOR(<<=);
  FRIEND_VEC_ASSIGN_OPERATOR(>>=);
  FRIEND_VEC_BOOLEAN_OPERATOR(==);
  FRIEND_VEC_BOOLEAN_OPERATOR(!=);
  FRIEND_VEC_BOOLEAN_OPERATOR(<);
  FRIEND_VEC_BOOLEAN_OPERATOR(>);
  FRIEND_VEC_BOOLEAN_OPERATOR(<=);
  FRIEND_VEC_BOOLEAN_OPERATOR(>=);
  FRIEND_VEC_UNARY_OPERATOR(!);
  FRIEND_VEC_UNARY_OPERATOR(~);

  static constexpr size_t size() {
    return NumElems;
  }

public:
  using element_type     = DataT;
  using __unspecified__  = void;
  using vector_t         = __unspecified__;
  using __swizzled_vec__ = __unspecified__;

  vec()                            = default;
  vec(const vec<DataT, NumElems>&) = default;
  vec(vec<DataT, NumElems>&& Rhs)  = default;

  explicit vec(const DataT& x) {
    for (size_t i(0); i < size(); i++)
      data_[i] = x;
  }

  template <size_t N = size(), typename = std::enable_if_t<(N == 1)>>
  vec(const DataT& x) {
    for (size_t i(0); i < size(); i++)
      data_[i] = x;
  }
  template <size_t N = size(), typename = std::enable_if_t<(N == 2)>>
  vec(const DataT& x, const DataT& y) {
    data_[0] = x;
    data_[1] = y;
  }
  template <size_t N = size(), typename = std::enable_if_t<(N == 3)>>
  vec(const DataT& x, const DataT& y, const DataT& z) {
    data_[0] = x;
    data_[1] = y;
    data_[2] = z;
  }
  template <size_t N = size(), typename = std::enable_if_t<(N == 4)>>
  vec(const DataT& x, const DataT& y, const DataT& z, const DataT& w) {
    data_[0] = x;
    data_[1] = y;
    data_[2] = z;
    data_[3] = w;
  }

  template <size_t N = size(), typename = std::enable_if_t<(N == 1)>>
  operator DataT() const {
    return data_[0];
  }

  size_t get_count() const {
    return size();
  }
  size_t get_size() const {
    return get_count() * sizeof(DataT);
  }

  template <typename convT, rounding_mode rm = rounding_mode::automatic>
  vec<convT, NumElems> convert() const {
    vec<convT, NumElems> r;
    // TODO: rounding mode for casting
    for (size_t i(0); i < size(); i++) {
      r[i] = convT(data_[i]);
    }
    return r;
  }

  template <typename asT>
  asT as() const;

  template <int... swizzleIndexes>
  __swizzled_vec__ swizzle() const;

  // Available only when numElements <= 4.
  // XYZW_ACCESS is: x, y, z, w, subject to numElements.
  __swizzled_vec__ XYZW_ACCESS() const;

  // Available only numElements == 4.
  // RGBA_ACCESS is: r, g, b, a.
  __swizzled_vec__ RGBA_ACCESS() const;

  // INDEX_ACCESS is: s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, sA, sB, sC, sD,
  // sE, sF, subject to numElements.
  __swizzled_vec__ INDEX_ACCESS() const;

#ifdef SYCL_SIMPLE_SWIZZLES
  // Available only when numElements <= 4.
  // XYZW_SWIZZLE is all permutations with repetition of: x, y, z, w, subject to
  // numElements.
  __swizzled_vec__ XYZW_SWIZZLE() const;

  // Available only when numElements == 4.
  // RGBA_SWIZZLE is all permutations with repetition of: r, g, b, a.
  __swizzled_vec__ RGBA_SWIZZLE() const;
#endif // #ifdef SYCL_SIMPLE_SWIZZLES

  __swizzled_vec__ lo() const;
  __swizzled_vec__ hi() const;
  __swizzled_vec__ odd() const;
  __swizzled_vec__ even() const;

  // load and store member functions
  // template <access::address_space addressSpace>
  // void load(size_t offset, multi_ptr<const DataT, addressSpace> ptr);
  // template <access::address_space addressSpace>
  // void store(size_t offset, multi_ptr<DataT, addressSpace> ptr) const;

  vec& operator=(const vec& Rhs) = default;

  vec& operator=(const DataT& Rhs) {
    for (size_t i = 0; i < size(); i++)
      data_[i] = Rhs;
  }

  vec& operator+=(const vec& Rhs) {
    *this = *this + Rhs;
    return *this;
  };

  element_type& x() {
    return data_[0];
  }
  template <size_t N = size(), typename = std::enable_if_t<(N > 1)>>
  element_type& y() {
    return data_[1];
  }
  template <size_t N = size(), typename = std::enable_if_t<(N > 2)>>
  element_type& z() {
    return data_[2];
  }
  template <size_t N = size(), typename = std::enable_if_t<(N > 3)>>
  element_type& w() {
    return data_[3];
  }
  inline element_type& operator[](size_t i) {
    return data_[i];
  }
  inline const element_type operator[](size_t i) const {
    return data_[i];
  }

private:
  element_type data_[NumElems];
};

DEF_VEC_BINARY_OPERATOR(+);
DEF_VEC_BINARY_OPERATOR(-);
DEF_VEC_BINARY_OPERATOR(*);
DEF_VEC_BINARY_OPERATOR(/);
DEF_VEC_BINARY_OPERATOR(%);
DEF_VEC_ASSIGN_OPERATOR(+=);
DEF_VEC_ASSIGN_OPERATOR(-=);
DEF_VEC_ASSIGN_OPERATOR(*=);
DEF_VEC_ASSIGN_OPERATOR(/=);
DEF_VEC_ASSIGN_OPERATOR(%=);
DEF_VEC_INCDEC_OPERATOR(++);
DEF_VEC_INCDEC_OPERATOR(--);
DEF_VEC_BINARY_OPERATOR(&);
DEF_VEC_BINARY_OPERATOR(|);
DEF_VEC_BINARY_OPERATOR(^);
DEF_VEC_ASSIGN_OPERATOR(&=);
DEF_VEC_ASSIGN_OPERATOR(|=);
DEF_VEC_ASSIGN_OPERATOR(^=);
DEF_VEC_BOOLEAN_OPERATOR(&&);
DEF_VEC_BOOLEAN_OPERATOR(||);
DEF_VEC_BINARY_OPERATOR(<<);
DEF_VEC_BINARY_OPERATOR(>>);
DEF_VEC_ASSIGN_OPERATOR(<<=);
DEF_VEC_ASSIGN_OPERATOR(>>=);
DEF_VEC_BOOLEAN_OPERATOR(==);
DEF_VEC_BOOLEAN_OPERATOR(!=);
DEF_VEC_BOOLEAN_OPERATOR(<);
DEF_VEC_BOOLEAN_OPERATOR(>);
DEF_VEC_BOOLEAN_OPERATOR(<=);
DEF_VEC_BOOLEAN_OPERATOR(>=);
DEF_VEC_UNARY_OPERATOR(!);
DEF_VEC_UNARY_OPERATOR(~);

using float4     = vec<float, 4>;
using float3     = vec<float, 3>;
using float2     = vec<float, 2>;
using double4    = vec<double, 4>;
using double3    = vec<double, 3>;
using double2    = vec<double, 2>;
using int4       = vec<int, 4>;
using int3       = vec<int, 3>;
using int2       = vec<int, 2>;
using uint4      = vec<uint, 4>;
using uint3      = vec<uint, 3>;
using uint2      = vec<uint, 2>;
using ulonglong  = unsigned long long;
using ulonglong4 = vec<ulonglong, 4>;
using ulonglong3 = vec<ulonglong, 3>;
using ulonglong2 = vec<ulonglong, 2>;

} // namespace neosycl::sycl
