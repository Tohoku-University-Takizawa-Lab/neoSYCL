#ifndef NEOSYCL_INCLUDE_NEOSYCL_SYCL_ND_RANGE_HPP_
#define NEOSYCL_INCLUDE_NEOSYCL_SYCL_ND_RANGE_HPP_

#include "id.hpp"

namespace neosycl::sycl {

template<int dimensions = 1>
class nd_range {
 public:
  nd_range(range<dimensions> globalSize, range<dimensions> localSize, id<dimensions> offset = id<dimensions>());

  range<dimensions> get_global_range() const;

  range<dimensions> get_local_range() const;

  range<dimensions> get_group_range() const;

  id<dimensions> get_offset() const;

};

}

#endif //NEOSYCL_INCLUDE_NEOSYCL_SYCL_ND_RANGE_HPP_
