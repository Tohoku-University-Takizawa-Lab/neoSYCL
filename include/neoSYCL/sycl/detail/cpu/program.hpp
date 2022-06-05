#pragma once

namespace neosycl::sycl::detail {

class program_data_cpu : public program_data {
  void* dll_;

public:
  friend class handler;
  program_data_cpu(device d) : program_data(d), dll_(nullptr) {}

  ~program_data_cpu() {
    if (dll_)
      dlclose(dll_);
  }

  bool open() override {
    const char* env = getenv(ENV_KERNEL);
    string_class fn(env ? env : DEFAULT_LIB);
    dll_ = dlopen(fn.c_str(), RTLD_LAZY);
    if (!dll_) {
      DEBUG_INFO("dlopen failed: %s", dlerror());
      return false;
    }
    DEBUG_INFO("kernel lib loaded: %lx, %s", (size_t)dll_, fn.c_str());
    return true;
  }

  bool is_open() override {
    return dll_ != nullptr;
  }

  void run(kernel k) override {
    auto kdc = cast<kernel_data_cpu>(k);

    for (const auto& acc : k.get_acc()) {
      acc.acquire_access();
    }
    try {
      DEBUG_INFO("-- KENREL EXEC BEGIN --");
      [[maybe_unused]] int ret_val = kdc->func_();
      DEBUG_INFO("-- KERNEL EXEC END (ret=%d) --", ret_val);
    }
    catch (exception& e) {
      PRINT_ERR("kernel execution failed: %s", e.what());
      throw;
    }
    for (const auto& acc : k.get_acc()) {
      acc.release_access();
    }
  }

  void* alloc_mem(void* p, size_t s, access::mode m) override {
    void* ret = malloc(s);
    if (m != access::mode::discard_write &&
        m != access::mode::discard_read_write)
      std::memcpy(ret, p, s);
    return ret;
  }

  void free_mem(void* p) override {
    if (p)
      free(p);
  }

  void copy_back(void* h, void* d, size_t s) override {
    std::memcpy(h, d, s);
  }

  void set_capture(kernel& k, void* p, size_t sz) override {
    auto kdc = cast<kernel_data_cpu>(k);
    if (kdc->capt_)
      std::memcpy(kdc->capt_, p, sz);
  }

  void set_range(kernel& k, size_t r[6]) override {
    auto kdc = cast<kernel_data_cpu>(k);
    if (kdc->rnge_)
      std::memcpy(kdc->rnge_, r, sizeof(size_t) * 6);
  }

  kernel_data_ptr create_kernel_data(const char* s) override {
    auto data = new kernel_data_cpu();
    // data->dll_        = dll_;
    string_class capt = string_class("__") + s + "_obj__";
    string_class rnge = string_class("__") + s + "_range__";
    auto f            = dlsym(dll_, s);
    data->func_       = reinterpret_cast<int (*)()>(f);
    data->capt_       = dlsym(dll_, capt.c_str());

    if (!data->func_ || !data->capt_) {
      PRINT_ERR("dlsym() for %s failed: %s", s, dlerror());
      throw exception("create_kernel() failed");
    }

    data->rnge_ = dlsym(dll_, rnge.c_str()); // this call could fail
    dlerror();                               // reset dlerror

    kernel_data_ptr ret(data);
    return ret;
  }
};
} // namespace neosycl::sycl::detail
