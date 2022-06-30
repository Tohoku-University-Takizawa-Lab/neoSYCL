#pragma once

namespace neosycl::sycl {

namespace info {

enum class kernel : int {
  function_name,
  num_args,
  context,
  program,
  reference_count,
  attributes
};

enum kernel_work_group : int {
  global_work_size,
  work_group_size,
  compile_work_group_size,
  preferred_work_group_size_multiple,
  private_mem_size
};
} // namespace info
} // namespace neosycl::sycl