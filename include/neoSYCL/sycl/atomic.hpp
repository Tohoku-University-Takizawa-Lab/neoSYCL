#pragma once

namespace neosycl::sycl {

template <typename T, access::address_space addressSpace =
                          access::address_space::global_space>
class atomic { /* TODO: not implemented yet */
public:
  atomic() = default;
};

} // namespace neosycl::sycl