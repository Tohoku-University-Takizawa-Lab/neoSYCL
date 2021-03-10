#ifndef NEOSYCL_INCLUDE_NEOSYCL_SYCL_ACCESSOR_HPP
#define NEOSYCL_INCLUDE_NEOSYCL_SYCL_ACCESSOR_HPP

#include "id.hpp"
#include "property_list.hpp"
#include "handler.hpp"
#include "detail/container/data_container.hpp"
#include "detail/container/data_container_nd.hpp"

namespace neosycl::sycl {

template<typename T, int dimensions, typename AllocatorT>
class buffer;

template<typename dataT, int dimensions, access::mode accessMode,
    access::target accessTarget = access::target::global_buffer,
    access::placeholder isPlaceholder = access::placeholder::false_t>
class accessor {

public:
  template<typename AllocatorT>
  accessor(buffer<dataT, dimensions, AllocatorT> &bufferRef, const property_list &propList = {}):
      data(bufferRef.data), accessRange(bufferRef.get_range()) {}

  template<typename AllocatorT>
  accessor(buffer<dataT, dimensions, AllocatorT> &bufferRef,
           range<dimensions> accessRange,
           const property_list &propList = {}):
      data(bufferRef.data), accessRange(accessRange) {}

  template<typename AllocatorT>
  accessor(buffer<dataT, dimensions, AllocatorT> &bufferRef,
           range<dimensions> accessRange,
           id<dimensions> accessOffset,
           const property_list &propList = {}
  ):data(bufferRef.data), accessRange(accessRange), accessOffset(accessOffset) {}

  template<typename AllocatorT>
  accessor(buffer<dataT, dimensions, AllocatorT> &bufferRef,
           handler &commandGroupHandlerRef,
           range<dimensions> accessRange,
           const property_list &propList = {}
  ):data(bufferRef.data), accessRange(accessRange), accessOffset(0) {}

  template<typename AllocatorT>
  accessor(buffer<dataT, dimensions, AllocatorT> &bufferRef,
           handler &commandGroupHandlerRef,
           range<dimensions> accessRange,
           id<dimensions> accessOffset,
           const property_list &propList = {}
  ):data(bufferRef.data), accessRange(accessRange), accessOffset(accessOffset) {}

  constexpr bool is_placeholder() const {
    return isPlaceholder;
  }

  size_t get_size() const {
    return data.get_size();
  }

  size_t get_count() const {
    return data.use_count();
  }

  range<dimensions> get_range() const {
    return accessRange;
  }

  id<dimensions> get_offset() const {
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
      ((Mode == access::mode::read_write) ||
          (Mode == access::mode::discard_write) ||
          (Mode == access::mode::discard_read_write)) &&
          (D > 0)>>
  dataT &operator[](id<dimensions> index) const {
    return (*data.get()).get(id2index(index));
  }

  /* Available only when: (accessMode == access::mode::write || accessMode == access::mode::read_write || accessMode == access::mode::discard_write || accessMode == access::mode::discard_read_write) && dimensions == 1) */
  template<access::mode Mode = accessMode, int D = dimensions, typename = std::enable_if_t<
      (Mode == access::mode::write) ||
          (Mode == access::mode::read_write) ||
          (Mode == access::mode::discard_write) ||
          (Mode == access::mode::discard_read_write) ||
          (D == 1)>>
  dataT &operator[](size_t index) const {
    return (*data.get())[index];
  }

  /* Available only when: dimensions > 1 */
  template<access::mode Mode = accessMode, int D = dimensions, typename = std::enable_if_t<
      ((Mode == access::mode::write) ||
          (Mode == access::mode::read_write) ||
          (Mode == access::mode::discard_write) ||
          (Mode == access::mode::discard_read_write)) &&
          (D == 2)>>
  dataT *operator[](size_t index) const {
    return data[index];
  }

  /* Available only when: dimensions > 1 */
  template<access::mode Mode = accessMode, int D = dimensions, typename = std::enable_if_t<
      ((Mode == access::mode::write) ||
          (Mode == access::mode::read_write) ||
          (Mode == access::mode::discard_write) ||
          (Mode == access::mode::discard_read_write)) &&
          (D == 3)>>
  dataT **operator[](size_t index) const {
    return data[index];
  }

  /* Available only when: accessMode == access::mode::read && dimensions == 0 */
  template<access::mode Mode = accessMode, int D = dimensions, typename = std::enable_if_t<
      (Mode == access::mode::read) && (D == 0)>>
  operator dataT() const;

  ~accessor() = default;

private:
  std::shared_ptr<detail::container::DataContainerND<dataT, dimensions>> data;
  range<dimensions> accessRange;
  id<dimensions> accessOffset;

  size_t id2index(id<dimensions> index) const {
    if (dimensions == 2) {
      return index[0] * index[1];
    } else if (dimensions == 3) {
      return index[0] * index[1] * index[2];
    }
    return index[0];
  }
};

}

#endif //NEOSYCL_INCLUDE_NEOSYCL_SYCL_ACCESSOR_HPP
