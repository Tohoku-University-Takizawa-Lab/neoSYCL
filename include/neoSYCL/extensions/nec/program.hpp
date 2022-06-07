#pragma once

namespace neosycl::sycl::extensions::nec {
const char* DEFAULT_VE_LIB    = "./kernel-ve.so";
const char* ENV_VE_KERNEL     = "NEOSYCL_VE_KERNEL";
constexpr int DEFAULT_VE_NODE = -1;

class program_data_ve : public detail::program_data {
  veo_util util;

public:
  program_data_ve(device d) : detail::program_data(d), util() {}

  ~program_data_ve() {
    util.close();
  }

  bool open() override {
    const char* env = getenv(ENV_VE_KERNEL);
    string_class fn(env ? env : DEFAULT_VE_LIB);

    if (util.open(fn, DEFAULT_VE_NODE) == false) {
      DEBUG_INFO("open() failed: %s", fn.c_str());
      return false;
    }
    DEBUG_INFO("kernel lib loaded: %s", fn.c_str());
    return true;
  }

  bool is_open() override {
    return util.is_open();
  }

  void run(kernel k) override {
    auto kdv = cast<kernel_data_ve>(k);

    try {
      DEBUG_INFO("-- KENREL EXEC BEGIN --");
      [[maybe_unused]] int rt = util.call_func(kdv->func_, kdv->argp_);
      DEBUG_INFO("-- KERNEL EXEC END (ret=%d) --", rt);
    }
    catch (exception& e) {
      PRINT_ERR("kernel execution failed: %s", e.what());
    }
  }

  void* alloc_mem(void* p, size_t s, access::mode m) override {
    void* dptr = util.alloc_mem(s);
    DEBUG_INFO("memory alloc: vaddr=%lx, size=%lu", (size_t)dptr, s);

    if (m != access::mode::discard_write &&
        m != access::mode::discard_read_write) {
      DEBUG_INFO("memory copy (h2v): "
                 "vaddr=%lx, haddr=%lx, size=%lu",
                 (size_t)dptr, (size_t)p, s);
      util.write_mem(dptr, p, s);
    }

    return dptr;
  }

  void free_mem(void* p) override {
    util.free_mem(p);
  }

  void write_mem(void* d, void* h, size_t s) override {
    util.write_mem(d, h, s);
  }

  void read_mem(void* h, void* d, size_t s) override {
    util.read_mem(h, d, s);
  }

  void copy_mem(void* d1, void* d2, size_t s) override {
    util.copy_mem(d1, d2, s);
  }

  void set_capture(kernel& k, void* p, size_t sz) override {
    auto kdv = cast<kernel_data_ve>(k);
    if (kdv->capt_)
      util.write_mem((void*)kdv->capt_, p, sz);
  }

  void set_range(kernel& k, size_t r[6]) override {
    auto kdv = cast<kernel_data_ve>(k);
    if (kdv->rnge_)
      util.write_mem((void*)kdv->rnge_, r, sizeof(size_t) * 6);
  }

  kernel_data_ptr create_kernel_data(const char* s) override {
    auto data = new kernel_data_ve();
    // data->dll_        = dll_;
    string_class capt = string_class("__") + s + "_obj__";
    string_class rnge = string_class("__") + s + "_range__";
    data->func_       = util.get_sym(s);
    data->capt_       = util.get_sym(capt.c_str());

    if (!data->func_ || !data->capt_) {
      PRINT_ERR("ve_dlfcn for %s failed", s);
      throw exception("create_kernel() failed");
    }

    data->rnge_ = util.get_sym(rnge.c_str()); // this call could fail

    kernel_data_ptr ret(data);
    return ret;
  }
};
} // namespace neosycl::sycl::extensions::nec
