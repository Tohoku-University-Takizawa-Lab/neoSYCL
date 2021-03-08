#ifndef NEOSYCL_INCLUDE_NEOSYCL_SYCL_ACCESSOR_IMPL_HPP
#define NEOSYCL_INCLUDE_NEOSYCL_SYCL_ACCESSOR_IMPL_HPP

#include "id.hpp"

namespace neosycl::sycl {

template<typename dataT, int dimensions, access::mode accessMode, access::placeholder isPlaceholder>
class accessor<dataT, dimensions, accessMode, access::target::global_buffer, isPlaceholder> {

public:
  template<typename AllocatorT>
  explicit accessor(buffer <dataT, dimensions, AllocatorT> &bufferRef, const property_list &propList = {}):
      bind_buffer(bufferRef), accessRange(bufferRef.get_range()), accessOffset(0) {}

  template<typename AllocatorT>
  accessor(buffer <dataT, dimensions, AllocatorT> &bufferRef,
           range <dimensions> accessRange,
           const property_list &propList = {}):
      bind_buffer(bufferRef), accessRange(accessRange), accessOffset(0) {}

  template<typename AllocatorT>
  accessor(buffer <dataT, dimensions, AllocatorT> &bufferRef,
           range <dimensions> accessRange,
           id <dimensions> accessOffset,
           const property_list &propList = {}
  ):bind_buffer(bufferRef), accessRange(accessRange), accessOffset(accessOffset) {}

  template<typename AllocatorT>
  accessor(buffer <dataT, dimensions, AllocatorT> &bufferRef,
           handler &commandGroupHandlerRef,
           range <dimensions> accessRange,
           const property_list &propList = {}
  ):bind_buffer(bufferRef), accessRange(accessRange), accessOffset(0) {}

  template<typename AllocatorT>
  accessor(buffer <dataT, dimensions, AllocatorT> &bufferRef,
           handler &commandGroupHandlerRef,
           range <dimensions> accessRange,
           id <dimensions> accessOffset,
           const property_list &propList = {}
  ):bind_buffer(bufferRef), accessRange(accessRange), accessOffset(accessOffset) {}

  constexpr bool is_placeholder() const {
    return isPlaceholder;
  }

  size_t get_size() const {
    return bind_buffer.get_size();
  }

  size_t get_count() const {
    return bind_buffer.get_count();
  }

  range <dimensions> get_range() const {
    return accessRange;
  }

  id <dimensions> get_offset() const {
    return accessOffset;
  }

  /* Available only when: (accessMode == access::mode::read_write || accessMode == access::mode::discard_read_write) && dimensions == 0) */
  template<access::mode Mode = accessMode, int D = dimensions, typename = std::enable_if_t<
      ((Mode == access::mode::read_write) ||
          (Mode == access::mode::discard_read_write)) &&
          (D == 0)>>
  operator dataT &() const;

  /* Available only when: (accessMode == access::mode::write || accessMode == access::mode::read_write || accessMode == access::mode::discard_write || accessMode == access::mode::discard_read_write) && dimensions > 0) */
  template<access::mode Mode = accessMode, int D = dimensions, typename = std::enable_if_t<
      (Mode == access::mode::read_write) ||
          (Mode == access::mode::discard_write) ||
          (Mode == access::mode::discard_read_write) ||
          (D > 0)>>
  dataT &operator[](id <dimensions> index) const {
    return bind_buffer.data->get(id2index(index));
  }

  /* Available only when: (accessMode == access::mode::write || accessMode == access::mode::read_write || accessMode == access::mode::discard_write || accessMode == access::mode::discard_read_write) && dimensions == 1) */
  template<access::mode Mode = accessMode, int D = dimensions, typename = std::enable_if_t<
      (Mode == access::mode::write) ||
          (Mode == access::mode::read_write) ||
          (Mode == access::mode::discard_write) ||
          (Mode == access::mode::discard_read_write) ||
          (D == 1)>>
  dataT &operator[](size_t index) const {
    return bind_buffer.data->get(index);
  }

  /* Available only when: accessMode == access::mode::read && dimensions == 0 */
  template<access::mode Mode = accessMode, int D = dimensions, typename = std::enable_if_t<
      (Mode == access::mode::read) && (D == 0)>>
  operator dataT() const;

  accessor(const accessor &rhs)
      : bind_buffer(rhs.bind_buffer), accessRange(rhs.accessRange), accessOffset(rhs.accessOffset) {}

  accessor(accessor &&rhs) : bind_buffer(rhs.bind_buffer), accessRange(rhs.accessRange), accessOffset(rhs.accessOffset) {}

  accessor &operator=(const accessor &rhs) {
    bind_buffer = rhs.bind_buffer;
    accessRange = rhs.accessRange;
    accessOffset = rhs.accessOffset;
  }

  accessor &operator=(accessor &&rhs) {
    bind_buffer = rhs.bind_buffer;
    accessRange = rhs.accessRange;
    accessOffset = rhs.accessOffset;
  }

  ~accessor() = default;

private:
  buffer <dataT, dimensions> bind_buffer;
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
