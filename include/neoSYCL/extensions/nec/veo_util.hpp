#pragma once
#include "ve_offload.h"

namespace neosycl::sycl::extensions::nec {

class veo_util {
  struct veo_proc_handle* proc_;
  struct veo_thr_ctxt* ctx_;
  uint64_t dll_;

public:
  veo_util() : proc_(nullptr), ctx_(nullptr), dll_(0) {}
  veo_util(const veo_util& u) = default;

  bool open(const string_class& lib_path, int ve_node) {
    proc_ = veo_proc_create(ve_node);
    if (!proc_) {
      DEBUG_INFO("veo_proc_create(%d) failed", ve_node);
      return false;
    }
    DEBUG_INFO("veo proc created: %lx", (size_t)proc_);

    dll_ = veo_load_library(proc_, lib_path.c_str());
    if (dll_ == 0) {
      DEBUG_INFO("veo_load_library failed: %s", lib_path.c_str());
      return false;
    }

    ctx_ = veo_context_open(proc_);
    DEBUG_INFO("veo_ctxt created: %lx", (size_t)ctx_);

    return true;
  }

  void close() {
    if (ctx_ == nullptr)
      return;
    DEBUG_INFO("veo ctxt released: %lx", (size_t)ctx_);
    int rt = veo_context_close(ctx_);
    if (rt != veo_command_state::VEO_COMMAND_OK) {
      PRINT_ERR("veo_context_close() failed (%d)", rt);
      throw runtime_error("veo_util::close() failed");
    }
    DEBUG_INFO("veo proc released: %lx", (size_t)proc_);
    rt = veo_proc_destroy(proc_);
    if (rt != veo_command_state::VEO_COMMAND_OK) {
      PRINT_ERR("veo_proc_destroy() failed (%d)", rt);
      throw runtime_error("veo_util::close() failed");
    }
  }

  bool is_open() const {
    return ctx_ != nullptr;
  }

  uint64_t get_sym(const char* s) {
    return veo_get_sym(proc_, dll_, s);
  }

  int write_mem(void* dst, const void* src, size_t sz) {
    int rt = veo_write_mem(proc_, (uint64_t)dst, src, sz);
    if (rt != VEO_COMMAND_OK) {
      PRINT_ERR("veo_write_mem() failed (%d)", rt);
      throw exception("veo_util::write_mem() failed");
    }
    return rt;
  }

  int read_mem(void* dst, const void* src, size_t sz) {
    int rt = veo_read_mem(proc_, dst, (uint64_t)src, sz);
    if (rt != VEO_COMMAND_OK) {
      PRINT_ERR("veo_read_mem() failed (%d)", rt);
      throw exception("veo_util::read_mem() failed");
    }
    return rt;
  }

  int copy_mem(void* dst, const void* src, size_t sz) {
    shared_ptr_class<unsigned char> tmp(new unsigned char[sz]);

    // the data is copied twice...
    int rt = veo_read_mem(proc_, tmp.get(), (uint64_t)src, sz);
    if (rt != VEO_COMMAND_OK) {
      PRINT_ERR("veo_copy_mem() failed (%d)", rt);
      throw exception("veo_util::copy_mem() failed");
    }
    rt = veo_write_mem(proc_, (uint64_t)dst, tmp.get(), sz);
    if (rt != VEO_COMMAND_OK) {
      PRINT_ERR("veo_copy_mem() failed (%d)", rt);
      throw exception("veo_util::copy_mem() failed");
    }
    return rt;
  }

  void* alloc_mem(size_t sz) {
    uint64_t ve_addr_int;

    int rt = veo_alloc_mem(proc_, &ve_addr_int, sz);
    if (rt != VEO_COMMAND_OK) {
      PRINT_ERR("veo_alloc_mem() failed (%d)", rt);
      throw exception("veo_util::alloc_mem() failed");
    }
    return (void*)ve_addr_int;
  }

  int free_mem(void* p) {
    return veo_free_mem(proc_, (uint64_t)p);
  }

  int call_func(uint64_t funcp, struct veo_args* argp) {
    uint64_t ret;
    uint64_t id = veo_call_async(ctx_, funcp, argp);
    int rt      = veo_call_wait_result(ctx_, id, &ret);
    if (rt != VEO_COMMAND_OK) {
      PRINT_ERR("veo_call_wait_result() failed (%d)", rt);
      throw exception("veo_util::call_func() failed");
    }
    return ret;
  }
};
} // namespace neosycl::sycl::extensions::nec
