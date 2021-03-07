#ifndef CUSTOM_SYCL_INCLUDE_SYCL_ACCESSOR_HPP_
#define CUSTOM_SYCL_INCLUDE_SYCL_ACCESSOR_HPP_

#include "types.hpp"
#include "handler.hpp"
#include "id.hpp"

namespace neosycl::sycl {

template<typename T, int dimensions, access::mode accessMode,
    access::target accessTarget = access::target::global_buffer,
    access::placeholder isPlaceholder = access::placeholder::false_t>
class accessor {};

}

#endif //CUSTOM_SYCL_INCLUDE_SYCL_ACCESSOR_HPP_
