#pragma once

namespace neosycl::sycl {

template <int dimensions = 1>
struct nd_range {
  nd_range(range<dimensions> globalSize, range<dimensions> localSize,
           id<dimensions> offset = id<dimensions>())
      : global_range(globalSize), local_range(localSize), offset(offset) {}

  range<dimensions> get_global_range() const {
    return global_range;
  }

  range<dimensions> get_local_range() const {
    return local_range;
  }

  range<dimensions> get_group_range();

  id<dimensions> get_offset() const {
    return offset;
  }

  range<dimensions> global_range;
  range<dimensions> local_range;
  id<dimensions> offset;
};

} // namespace neosycl::sycl
