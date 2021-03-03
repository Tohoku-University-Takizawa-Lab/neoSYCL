#ifndef NEOSYCL_INCLUDE_NEOSYCL_SYCL_ACCESSOR_IMPL_HPP
#define NEOSYCL_INCLUDE_NEOSYCL_SYCL_ACCESSOR_IMPL_HPP

#include "id.hpp"

namespace neosycl::sycl {

template<typename T, int dimensions, access::mode accessMode, access::placeholder isPlaceholder>
class accessor<T, dimensions, accessMode, access::target::global_buffer, isPlaceholder> {

public:
  template<typename AllocatorT>
  explicit accessor(buffer <T, dimensions, AllocatorT> &bufferRef, const property_list &propList = {}):
      bufferRef(bufferRef), accessRange(bufferRef.get_range()), accessOffset(0) {}

  template<typename AllocatorT>
  accessor(buffer <T, dimensions, AllocatorT> &bufferRef,
           range <dimensions> accessRange,
           const property_list &propList = {}):
      bufferRef(bufferRef), accessRange(accessRange), accessOffset(0) {}

  template<typename AllocatorT>
  accessor(buffer <T, dimensions, AllocatorT> &bufferRef,
           range <dimensions> accessRange,
           id <dimensions> accessOffset,
           const property_list &propList = {}
  ):bufferRef(bufferRef), accessRange(accessRange), accessOffset(accessOffset) {}

  template<typename AllocatorT>
  accessor(buffer <T, dimensions, AllocatorT> &bufferRef,
           handler &commandGroupHandlerRef,
           range <dimensions> accessRange,
           const property_list &propList = {}
  ):bufferRef(bufferRef), accessRange(accessRange), accessOffset(0) {}

  template<typename AllocatorT>
  accessor(buffer <T, dimensions, AllocatorT> &bufferRef,
           handler &commandGroupHandlerRef,
           range <dimensions> accessRange,
           id <dimensions> accessOffset,
           const property_list &propList = {}
  ):bufferRef(bufferRef), accessRange(accessRange), accessOffset(accessOffset) {}

  constexpr bool is_placeholder() const {
    return isPlaceholder;
  }

  size_t get_size() const {
    return bufferRef.get_size();
  }

  size_t get_count() const {
    return bufferRef.get_count();
  }

  range <dimensions> get_range() const {
    return accessRange;
  }

  id <dimensions> get_offset() const {
    return accessOffset;
  }

  operator T &() const;

  T operator[](id <dimensions> index) const {
    return bufferRef.data[id2index(index)];
  }

  operator T() const;

private:
  buffer <T, dimensions> &bufferRef;
  range <dimensions> accessRange;
  id <dimensions> accessOffset;

  size_t id2index(id <dimensions> index) {
    if (dimensions == 2) {
      return index[0] * index[1];
    } else if (dimensions == 3) {
      return index[0] * index[1] * index[2];
    }
    return index[0];
  }
};

}

#endif //NEOSYCL_INCLUDE_NEOSYCL_SYCL_ACCESSOR_IMPL_HPP
