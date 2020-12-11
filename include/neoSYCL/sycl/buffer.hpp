#ifndef CUSTOM_SYCL_INCLUDE_SYCL_BUFFER_HPP_
#define CUSTOM_SYCL_INCLUDE_SYCL_BUFFER_HPP_

#include <shared_mutex>

#include "types.hpp"
#include "range.hpp"
#include "access.hpp"
#include "accessor.hpp"
#include "allocator.hpp"
#include "handler.hpp"
#include "property_list.hpp"
#include "buffer/data_container.hpp"
#include "buffer/data_container_1d.hpp"
#include "buffer/data_container_2d.hpp"
#include "buffer/data_container_3d.hpp"
#include "detail/shared_ptr_implementation.hpp"

namespace neosycl::sycl {

template<typename T, std::size_t Dimensions = 1>
class buffer : public detail::SharedPtrImplementation<detail::DataContainerND<T, Dimensions>> {
  using value_type = T;
  using reference = value_type &;
  using const_reference = const value_type &;

  using base_class = typename detail::SharedPtrImplementation<detail::DataContainerND<T, Dimensions>>;

  friend accessor<T, Dimensions, access::mode::read, access::target::global_buffer>;
  friend accessor<T, Dimensions, access::mode::write, access::target::global_buffer>;
  friend accessor<T, Dimensions, access::mode::read_write, access::target::global_buffer>;

 public:

  // Construct a SYCL buffer instance with uninitialized memory.
  buffer(const range<Dimensions> &bufferRange, const property_list &propList = {}) :
      base_class(new detail::DataContainerND<T, Dimensions>(bufferRange)) {}

  template<typename Iterator>
  buffer(Iterator first, Iterator last, const property_list &propList = {}):
      base_class(new detail::DataContainerND<T, Dimensions>(first, last)) {}

  // Construct a SYCL buffer instance with the hostData parameter provided.
  buffer(T *hostData, const range<Dimensions> &bufferRange, const property_list &propList = {}) :
      base_class(new detail::DataContainerND<T, Dimensions>(hostData, bufferRange)) {}

  // Construct a SYCL buffer instance with the hostData parameter provided. The ownership of this memory is given to
  // the constructed SYCL buffer for the duration of its lifetime.
  buffer(const T *hostData, const range<Dimensions> &bufferRange, const property_list &propList = {}) :
      base_class(new detail::DataContainerND<T, Dimensions>(hostData, bufferRange)) {}

  // Return a range object representing the size of the buffer in terms of number of elements
  // in each dimension as passed to the constructor.
  range<Dimensions> get_range() {
    return this->implementation->get_range();
  }

  // Returns the total number of elements in the buffer.
  size_t get_count() {
    return this->implementation->get_range().size();
  }

  // Returns the size of the buffer storage in bytes.
  // Equal to get_count()*sizeof(T).
  size_t get_size() {
    return this->implementation->get_size();
  }

  template<access::mode mode, access::target target = access::target::global_buffer>
  accessor<T, Dimensions, mode, target> get_access(handler &commandGroupHandler) {
    return accessor<T, Dimensions, mode, target>(*this, commandGroupHandler);
  }

  template<access::mode mode, access::target target = access::target::global_buffer>
  accessor<T, Dimensions, mode, target> get_access() {
    return accessor<T, Dimensions, mode, target>(*this);
  }
};

}

#endif //CUSTOM_SYCL_INCLUDE_SYCL_BUFFER_HPP_
