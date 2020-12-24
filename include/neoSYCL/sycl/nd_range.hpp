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

 private:
  range<dimensions> global_range;
  range<dimensions> local_range;
  id<dimensions> offset;
};

template<int dimensions>
nd_range<dimensions>::nd_range(range<dimensions> globalSize,
                               range<dimensions> localSize,
                               id<dimensions> offset)
    :global_range(globalSize), local_range(localSize), offset(offset) {}

template<int dimensions>
range<dimensions> nd_range<dimensions>::get_global_range() const {
  return global_range;
}
template<int dimensions>
range<dimensions> nd_range<dimensions>::get_local_range() const {
  return local_range;
}
template<int dimensions>
range<dimensions> nd_range<dimensions>::get_group_range() const {

  return range<dimensions>(0);
}
template<int dimensions>
id<dimensions> nd_range<dimensions>::get_offset() const {
  return id<dimensions>();
}

}

#endif //NEOSYCL_INCLUDE_NEOSYCL_SYCL_ND_RANGE_HPP_
