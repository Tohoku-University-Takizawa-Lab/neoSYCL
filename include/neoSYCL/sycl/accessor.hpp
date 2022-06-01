#pragma once

#include "neoSYCL/sycl/detail/container/data_container.hpp"
#include "neoSYCL/sycl/detail/container/data_container_nd.hpp"

namespace neosycl::sycl {

// prototype decls
template <typename T, size_t dimensions, typename AllocatorT> class buffer;
class handler;

///////////////////////////////////////////////////////////////////////////////
template <typename dataT, size_t dimensions, access::mode accessMode,
          access::target accessTarget       = access::target::global_buffer,
          access::placeholder isPlaceholder = access::placeholder::false_t>
class accessor {
  friend class handler;

public:
  using value_type      = dataT;
  using reference       = dataT&;
  using const_reference = const dataT&;
  using container_type  = detail::container::DataContainerND<dataT, dimensions>;

  accessor(const accessor& rhs) = default;
  accessor(accessor&& rhs)      = default;
  ~accessor()                   = default;

  accessor& operator=(const accessor& rhs) = default;
  accessor& operator=(accessor&& rhs) = default;

  template <typename T, size_t D, access::mode M, access::target A,
            access::placeholder P>
  friend bool operator==(const accessor<T, D, M, A, P>& lhs,
                         const accessor<T, D, M, A, P>& rhs);
  template <typename T, size_t D, access::mode M, access::target A,
            access::placeholder P>
  friend bool operator!=(const accessor<T, D, M, A, P>& lhs,
                         const accessor<T, D, M, A, P>& rhs);

#if 0
  /* TODO: accessor of dimensions == 0 is not supported yet */

  /* Available only when: ((isPlaceholder == access::placeholder::false_t &&
  accessTarget == access::target::host_buffer) || (isPlaceholder ==
  access::placeholder::true_t && (accessTarget == access::target::global_buffer
  || accessTarget == access::target::constant_buffer))) && dimensions == 0 */
  template <typename AllocatorT, size_t D = dimensions,
            typename = std::enable_if_t<(D == 0)>>
  accessor(buffer<dataT, 1, AllocatorT>& bufferRef,
           const property_list& propList = {})
      : data(bufferRef.data), accessRange(bufferRef.get_range()),
        accessOffset(), device_ptr(nullptr) {}

  /* Available only when: (isPlaceholder == access::placeholder::false_t &&
     (accessTarget == access::target::global_buffer || accessTarget ==
     access::target::constant_buffer)) && dimensions == 0 */
  template <typename AllocatorT, size_t D = dimensions,
            typename = std::enable_if_t<(D == 0)>>
  accessor(buffer<dataT, 1, AllocatorT>& bufferRef,
           handler& commandGroupHandlerRef, const property_list& propList = {})
      : data(bufferRef.data), accessRange(bufferRef.get_range()),
        accessOffset(), device_ptr(nullptr) {
    alloc_(commandGroupHandlerRef);
  }
#endif

  /* Available only when: ((isPlaceholder == access::placeholder::false_t &&
   accessTarget == access::target::host_buffer) || (isPlaceholder ==
   access::placeholder::true_t && (accessTarget == access::target::global_buffer
   || accessTarget == access::target::constant_buffer))) && dimensions > 0 */
  template <typename AllocatorT, size_t D = dimensions,
            typename = std::enable_if_t<(D > 0)>>
  accessor(buffer<dataT, dimensions, AllocatorT>& bufferRef,
           const property_list& propList = {})
      : data(bufferRef.data), accessRange(bufferRef.get_range()),
        accessOffset(), device_ptr(nullptr) {}

  /* Available only when: (isPlaceholder == access::placeholder::false_t &&
   (accessTarget == access::target::global_buffer || accessTarget ==
   access::target::constant_buffer)) && dimensions > 0 */
  template <typename AllocatorT, size_t D = dimensions,
            typename = std::enable_if_t<(D > 0)>>
  accessor(buffer<dataT, dimensions, AllocatorT>& bufferRef,
           handler& commandGroupHandlerRef, const property_list& propList = {})
      : data(bufferRef.data), accessRange(bufferRef.get_range()),
        accessOffset(), device_ptr(nullptr) {
    alloc_(commandGroupHandlerRef);
  }

  /* Available only when: (isPlaceholder == access::placeholder::false_t &&
   accessTarget == access::target::host_buffer) || (isPlaceholder ==
  access::placeholder::true_t && (accessTarget == access::target::global_buffer
   || accessTarget == access::target::constant_buffer)) && dimensions > 0 */
  template <typename AllocatorT, size_t D = dimensions,
            typename = std::enable_if_t<(D > 0)>>
  accessor(buffer<dataT, dimensions, AllocatorT>& bufferRef,
           range<dimensions> accessRange, const property_list& propList = {})
      : data(bufferRef.data), accessRange(accessRange), accessOffset(),
        device_ptr(nullptr) {}

  /* Available only when: (isPlaceholder == access::placeholder::false_t &&
   accessTarget == access::target::host_buffer) || (isPlaceholder ==
   access::placeholder::true_t && (accessTarget == access::target::global_buffer
  || accessTarget == access::target::constant_buffer)) && dimensions > 0 */
  template <typename AllocatorT, size_t D = dimensions,
            typename = std::enable_if_t<(D > 0)>>
  accessor(buffer<dataT, dimensions, AllocatorT>& bufferRef,
           range<dimensions> accessRange, id<dimensions> accessOffset,
           const property_list& propList = {})
      : data(bufferRef.data), accessRange(accessRange),
        accessOffset(accessOffset), device_ptr(nullptr) {}

  /* Available only when: (isPlaceholder == access::placeholder::false_t &&
  (accessTarget == access::target::global_buffer || accessTarget ==
  access::target::constant_buffer)) && dimensions > 0 */
  template <typename AllocatorT, size_t D = dimensions,
            typename = std::enable_if_t<(D > 0)>>
  accessor(buffer<dataT, dimensions, AllocatorT>& bufferRef,
           handler& commandGroupHandlerRef, range<dimensions> accessRange,
           const property_list& propList = {})
      : data(bufferRef.data), accessRange(accessRange), accessOffset(),
        device_ptr(nullptr) {
    alloc_(commandGroupHandlerRef);
  }

  /* Available only when: (isPlaceholder == access::placeholder::false_t &&
  (accessTarget == access::target::global_buffer || accessTarget ==
  access::target::constant_buffer)) && dimensions > 0 */
  template <typename AllocatorT, size_t D = dimensions,
            typename = std::enable_if_t<(D > 0)>>
  accessor(buffer<dataT, dimensions, AllocatorT>& bufferRef,
           handler& commandGroupHandlerRef, range<dimensions> accessRange,
           id<dimensions> accessOffset, const property_list& propList = {})
      : data(bufferRef.data), accessRange(accessRange),
        accessOffset(accessOffset), device_ptr(nullptr) {
    alloc_(commandGroupHandlerRef);
  }

  /* -- common interface members -- */

  /* -- property interface members -- */
  constexpr bool is_placeholder() const { return isPlaceholder; }

  size_t get_size() const { return data->get_size(); }

  size_t get_count() const { return data->get_count(); }

  range<dimensions> get_range() const { return accessRange; }

  id<dimensions> get_offset() const { return accessOffset; }

  /* Available only when: (accessMode == access::mode::read_write ||
   * accessMode
   * == access::mode::discard_read_write) && dimensions == 0) */
  template <access::mode Mode = accessMode, size_t D = dimensions,
            typename =
                std::enable_if_t<((Mode == access::mode::read_write) ||
                                  (Mode == access::mode::discard_read_write)) &&
                                 (D == 0)>>
  operator dataT&() const;

  /* Available only when: (accessMode == access::mode::write || accessMode ==
   * access::mode::read_write || accessMode == access::mode::discard_write ||
   * accessMode == access::mode::discard_read_write) && dimensions > 0) */
  template <access::mode Mode = accessMode, size_t D = dimensions,
            typename =
                std::enable_if_t<((Mode == access::mode::write) ||
                                  (Mode == access::mode::read_write) ||
                                  (Mode == access::mode::discard_write) ||
                                  (Mode == access::mode::discard_read_write)) &&
                                 (D > 0)>>
  dataT& operator[](id<dimensions> index) const {
    size_t index_val = id2index(index);
    DEBUG_INFO("access with index: %d", index_val);
    return (*data).get(index_val);
  }

  template <
      access::mode Mode = accessMode, size_t D = dimensions,
      typename = std::enable_if_t<(Mode == access::mode::read) && (D > 0)>>
  dataT operator[](id<dimensions> index) const {
    size_t index_val = id2index(index);
    DEBUG_INFO("read access with index: %d", index_val);
    return (*data).get(index_val);
  }

  /* Available only when: (accessMode == access::mode::write || accessMode ==
   * access::mode::read_write || accessMode == access::mode::discard_write ||
   * accessMode == access::mode::discard_read_write) && dimensions == 1) */
  template <access::mode Mode = accessMode, size_t D = dimensions,
            typename =
                std::enable_if_t<((Mode == access::mode::write) ||
                                  (Mode == access::mode::read_write) ||
                                  (Mode == access::mode::discard_write) ||
                                  (Mode == access::mode::discard_read_write)) &&
                                 (D == 1)>>
  dataT& operator[](size_t index) const {
    return (*data)[index];
  }

  template <
      access::mode Mode = accessMode, size_t D = dimensions,
      typename = std::enable_if_t<(Mode == access::mode::read) && (D == 1)>>
  dataT operator[](size_t index) const {
    return (*data)[index];
  }

  /* Available only when: dimensions > 1 */
  template <access::mode Mode = accessMode, size_t D = dimensions,
            typename =
                std::enable_if_t<((Mode == access::mode::write) ||
                                  (Mode == access::mode::read_write) ||
                                  (Mode == access::mode::discard_write) ||
                                  (Mode == access::mode::discard_read_write)) &&
                                 (D == 2)>>
  dataT* operator[](size_t index) const {
    return (*data)[index];
  }

  template <
      access::mode Mode = accessMode, size_t D = dimensions,
      typename = std::enable_if_t<(Mode == access::mode::read) && (D == 2)>>
  const dataT* operator[](size_t index) const {
    return (*data)[index];
  }

  /* Available only when: dimensions > 1 */
  template <access::mode Mode = accessMode, size_t D = dimensions,
            typename =
                std::enable_if_t<((Mode == access::mode::write) ||
                                  (Mode == access::mode::read_write) ||
                                  (Mode == access::mode::discard_write) ||
                                  (Mode == access::mode::discard_read_write)) &&
                                 (D == 3)>>
  detail::container::AccessProxyND<dataT, 3> operator[](size_t index) const {
    return (*data)[index];
  }

  template <
      access::mode Mode = accessMode, size_t D = dimensions,
      typename = std::enable_if_t<(Mode == access::mode::read) && (D == 3)>>
  const dataT** operator[](size_t index) const {
    return (*data)[index];
  }

  /* Available only when: accessMode == access::mode::read && dimensions == 0
   */
  template <
      access::mode Mode = accessMode, size_t D = dimensions,
      typename = std::enable_if_t<(Mode == access::mode::read) && (D == 0)>>
  operator dataT() const;

  template <access::target T = accessTarget,
            typename = std::enable_if_t<T == access::target::host_buffer>>
  dataT* get_pointer() const {
    return data->get_ptr();
  }

  /* Available only when: accessMode == access::mode::atomic && dimensions ==
  0 */
  template <
      access::mode Mode = accessMode, size_t D = dimensions,
      typename = std::enable_if_t<(Mode == access::mode::atomic) && (D == 0)>>
  operator atomic<dataT, access::address_space::global_space>() const;

  /* Available only when: accessMode == access::mode::atomic && dimensions >
  0 */
  template <
      access::mode Mode = accessMode, size_t D = dimensions,
      typename = std::enable_if_t<(Mode == access::mode::atomic) && (D > 0)>>
  atomic<dataT, access::address_space::global_space>
  operator[](id<dimensions> index) const;

  /* Available only when: accessMode == access::mode::atomic && dimensions ==
  1 */
  template <
      access::mode Mode = accessMode, size_t D = dimensions,
      typename = std::enable_if_t<(Mode == access::mode::atomic) && (D == 1)>>
  atomic<dataT, access::address_space::global_space>
  operator[](size_t index) const;

  /* Available only when: dimensions > 1 */
  // __unspecified__& operator[](size_t index) const;

  template <access::target T = accessTarget,
            typename = std::enable_if_t<(T == access::target::global_buffer) ||
                                        (T == access::target::constant_buffer)>>
  void* get_pointer() const;

private:
  std::shared_ptr<detail::container::DataContainerND<dataT, dimensions>> data;
  range<dimensions> accessRange;
  id<dimensions> accessOffset;
  void* device_ptr;

  size_t id2index(id<dimensions> index) const {
    size_t x = this->accessRange.get(0);
    size_t y = this->accessRange.get(1);
    if (dimensions == 2) {
      return index[0] + x * index[1];
    }
    else if (dimensions == 3) {
      return index[0] + x * (index[1] + y * index[2]);
    }
    return index[0];
  }

  void alloc_(handler& h) /* defined in handler.hpp */;
};

template <typename T, size_t D, access::mode M, access::target A,
          access::placeholder P>
bool operator==(const accessor<T, D, M, A, P>& lhs,
                const accessor<T, D, M, A, P>& rhs) {
  return lhs.data == rhs.data && lhs.accessRange == rhs.accessRange &&
         lhs.accessOffset == rhs.accessOffset;
}

template <typename T, size_t D, access::mode M, access::target A,
          access::placeholder P>
bool operator!=(const accessor<T, D, M, A, P>& lhs,
                const accessor<T, D, M, A, P>& rhs) {
  return !(lhs == rhs);
}
} // namespace neosycl::sycl
