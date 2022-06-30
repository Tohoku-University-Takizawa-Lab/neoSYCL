#ifndef SYCL_INCLUDE_CL_SYCL_PROPERTY_LIST_HPP_
#define SYCL_INCLUDE_CL_SYCL_PROPERTY_LIST_HPP_

namespace neosycl::sycl {

class property_list {

  template <typename propertyT>
  bool has_property() const {
    throw unimplemented();
  }

  template <typename propertyT>
  propertyT get_property() const {
    throw unimplemented();
  }
};

} // namespace neosycl::sycl

#endif // SYCL_INCLUDE_CL_SYCL_PROPERTY_LIST_HPP_
