//
// Created by WhiteBlue on 2020/5/27.
//

#ifndef SYCL_INCLUDE_CL_SYCL_PROPERTY_LIST_HPP_
#define SYCL_INCLUDE_CL_SYCL_PROPERTY_LIST_HPP_

namespace cl::sycl {

class property_list {

  template<typename propertyT>
  bool has_property() const {
    throw UnimplementedException();
  }

  template<typename propertyT>
  propertyT get_property() const {
    throw UnimplementedException();
  }

};

}

#endif //SYCL_INCLUDE_CL_SYCL_PROPERTY_LIST_HPP_
