#pragma once

namespace neosycl::sycl::info {

template <typename T, T Param>
class param_traits {
  using return_type = T;
};

template <info::platform Param>
class param_traits<info::platform, Param> {
  using type = string_class;
};

template <info::device Param>
class param_traits<info::device, Param> {
  using type = string_class;
};

} // namespace neosycl::sycl::info
