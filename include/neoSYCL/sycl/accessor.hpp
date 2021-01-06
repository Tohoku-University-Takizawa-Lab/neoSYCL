#ifndef CUSTOM_SYCL_INCLUDE_SYCL_ACCESSOR_HPP_
#define CUSTOM_SYCL_INCLUDE_SYCL_ACCESSOR_HPP_

#include "buffer.hpp"
#include "types.hpp"
#include "handler.hpp"
#include "id.hpp"

namespace neosycl::sycl {

template<typename T, int dimensions, typename AllocatorT>
class buffer;

template<typename dataT, int dimensions, access::mode accessmode,
    access::target accessTarget = access::target::global_buffer,
    access::placeholder isPlaceholder = access::placeholder::false_t>
class accessor {};



template<typename dataT, int dimensions>
class accessor<dataT, dimensions, access::mode::read, access::target::global_buffer, access::placeholder::false_t> {

  template<typename AllocatorT>
  accessor(buffer<dataT, dimensions, AllocatorT> &bufferRef, const property_list &propList = {});

  template<typename AllocatorT>
  accessor(buffer<dataT, dimensions, AllocatorT> &bufferRef,
           range<dimensions> accessRange, const property_list &propList = {});

  template<typename AllocatorT>
  accessor(buffer<dataT, dimensions, AllocatorT> &bufferRef,
           range<dimensions> accessRange, id<dimensions> accessOffset, const property_list &propList = {});

  template<typename AllocatorT>
  accessor(buffer<dataT, dimensions, AllocatorT> &bufferRef,
           handler &commandGroupHandlerRef, range<dimensions> accessRange, const property_list &propList = {});

  template<typename AllocatorT>
  accessor(buffer<dataT, dimensions, AllocatorT> &bufferRef,
           handler &commandGroupHandlerRef,
           range<dimensions> accessRange,
           id<dimensions> accessOffset,
           const property_list &propList = {});

  constexpr bool is_placeholder() const;

  size_t get_size() const;

  size_t get_count() const;

  range<dimensions> get_range() const;

  id<dimensions> get_offset() const;

  operator dataT &() const;

  dataT operator[](id<dimensions> index) const;

  operator dataT() const;
};

template<typename dataT, int dimensions>
class accessor<dataT, dimensions, access::mode::write, access::target::global_buffer, access::placeholder::false_t> {

  template<typename AllocatorT>
  accessor(buffer<dataT, dimensions, AllocatorT> &bufferRef, const property_list &propList = {});

  template<typename AllocatorT>
  accessor(buffer<dataT, dimensions, AllocatorT> &bufferRef,
           range<dimensions> accessRange, const property_list &propList = {});

  template<typename AllocatorT>
  accessor(buffer<dataT, dimensions, AllocatorT> &bufferRef,
           range<dimensions> accessRange, id<dimensions> accessOffset, const property_list &propList = {});

  template<typename AllocatorT>
  accessor(buffer<dataT, dimensions, AllocatorT> &bufferRef,
           handler &commandGroupHandlerRef, range<dimensions> accessRange, const property_list &propList = {});

  template<typename AllocatorT>
  accessor(buffer<dataT, dimensions, AllocatorT> &bufferRef,
           handler &commandGroupHandlerRef,
           range<dimensions> accessRange,
           id<dimensions> accessOffset,
           const property_list &propList = {});

  constexpr bool is_placeholder() const;

  size_t get_size() const;

  size_t get_count() const;

  range<dimensions> get_range() const;

  id<dimensions> get_offset() const;

  operator dataT &() const;

  dataT operator[](id<dimensions> index) const;

  operator dataT() const;
};

}

#endif //CUSTOM_SYCL_INCLUDE_SYCL_ACCESSOR_HPP_
