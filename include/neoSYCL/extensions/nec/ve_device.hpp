#ifndef SYCL_INCLUDE_CL_SYCL_NEC_VE_DEVICE_HPP_
#define SYCL_INCLUDE_CL_SYCL_NEC_VE_DEVICE_HPP_

#include "ve_offload.h"
#include "neoSYCL/sycl/device.hpp"

namespace neosycl::sycl {

class ve_device : public device {
private:
  int node_id;

public:
  ve_device(int node_id) : node_id(node_id) {}

  bool is_host() const override { return false; }
  bool is_cpu() const override { return false; }
  bool is_gpu() const override { return false; }
  bool is_accelerator() const override { return true; }
  int get_node_id() const { return node_id; }
};

} // namespace neosycl::sycl

#endif // SYCL_INCLUDE_CL_SYCL_NEC_VE_DEVICE_HPP_
