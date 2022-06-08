#pragma once

namespace neosycl::sycl::info {

template <typename T, T Param>
struct param_traits {
  using return_type = int;
  static const return_type value;
};

#define DEF_STR_INFO_TRAIT(Info, Param, Value)                                 \
  template <>                                                                  \
  struct param_traits<Info, Param> {                                           \
    using return_type             = string_class;                              \
    static constexpr char value[] = Value;                                     \
  };                                                                           \
  // const char param_traits<Info, Param>::value[] = Value;

#define DEF_INFO_TRAIT(Info, Param, Type, Value)                               \
  template <>                                                                  \
  struct param_traits<Info, Param> {                                           \
    using return_type                  = Type;                                 \
    inline static constexpr Type value = Type(Value);                          \
  };

#define DEF_INFO_TYPE_TRAIT(Info, Param, Type)                                 \
  template <>                                                                  \
  struct param_traits<Info, Param> {                                           \
    using return_type = Type;                                                  \
  };

} // namespace neosycl::sycl::info
