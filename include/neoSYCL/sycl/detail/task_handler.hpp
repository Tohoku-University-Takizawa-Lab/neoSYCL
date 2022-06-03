#ifndef NEOSYCL_INCLUDE_NEOSYCL_SYCL_TASK_HANDLER_HPP
#define NEOSYCL_INCLUDE_NEOSYCL_SYCL_TASK_HANDLER_HPP

#include "neoSYCL/sycl/kernel.hpp"
#include <cstring>

namespace neosycl::sycl::detail {

class task_handler {
protected:
  using container_ptr    = shared_ptr_class<detail::container::DataContainer>;
  using kernel_info_type = kernel::info_type;

public:
  explicit task_handler() {}

  virtual void run(kernel k) = 0;

  // virtual SUPPORT_PLATFORM_TYPE type() = 0;

  virtual void* get_pointer(container_ptr)                                  = 0;
  virtual void* alloc_mem(container_ptr, access::mode = access::mode::read) = 0;
  virtual void free_mem(container_ptr)                                      = 0;
  virtual void copy_back()                                                  = 0;

  virtual void set_capture(kernel&, void* p, size_t sz) = 0;
  virtual void set_range(kernel&, size_t r[6])          = 0;

  template <size_t dim>
  void set_range(kernel& k, range<dim> r) {
    size_t sz[6] = {1, 1, 1, 0, 0, 0};
    for (size_t idx(0); idx != dim; idx++) {
      sz[idx] = r[idx];
    }
    set_range(k, sz);
  }

  template <size_t dim>
  void set_range(kernel& k, range<dim> r, id<dim> i) {
    size_t sz[6] = {1, 1, 1, 0, 0, 0};
    for (size_t idx(0); idx != dim; idx++) {
      sz[idx] = r[idx];
    }
    for (size_t idx(3); idx != dim + 3; idx++) {
      sz[idx] = i[idx];
    }
    set_range(k, sz);
  }

  virtual kernel create_kernel(const char*) = 0;
};

class task_handler_cpu : public task_handler {
  void* dll_;

public:
  task_handler_cpu(void* p) : dll_(p) {}

#if 0
    void single_task(kernel& k,
      const std::function<void(void)>& func) override {
      for (const accessor_info& arg : k->get_acc()) {
        arg.acquire_access();
      }
      DEBUG_INFO("single_task(): %s\n", k->get_name());
      func();
      for (const accessor_info& arg : k->get_acc()) {
        arg.release_access();
      }
    }

    void parallel_for_1d(kernel& k, range<1> r,
      const std::function<void(id<1>)>& func,
      id<1> offset) override {
      for (const accessor_info& arg : k->get_acc()) {
        arg.acquire_access();
      }
      DEBUG_INFO("parallel_for_1d(): %s\n", k->get_name());
      for (size_t x = offset.get(0); x < r.get(0); x++) {
        func(id<1>(x));
      }
      for (const accessor_info& arg : k->get_acc()) {
        arg.release_access();
      }
    };

    void parallel_for_2d(kernel& k, range<2> r,
      const std::function<void(id<2>)>& func,
      id<2> offset) override {
      for (const accessor_info& arg : k->get_acc()) {
        arg.acquire_access();
      }
      DEBUG_INFO("parallel_for_2d(): %s\n", k->get_name());
      for (size_t x = offset.get(0); x < r.get(0); x++) {
        for (size_t y = offset.get(1); y < r.get(1); y++) {
          func(id<2>(x, y));
        }
      }
      for (const accessor_info& arg : k->get_acc()) {
        arg.release_access();
      }
    };

    void parallel_for_3d(kernel& k, range<3> r,
      const std::function<void(id<3>)>& func,
      id<3> offset) override {
      for (const accessor_info& arg : k->get_acc()) {
        arg.acquire_access();
      }
      DEBUG_INFO("parallel_for_3d(): %s\n", k->get_name());
      for (size_t x = offset.get(0); x < r.get(0); x++) {
        for (size_t y = offset.get(1); y < r.get(1); y++) {
          for (size_t z = offset.get(2); z < r.get(2); z++) {
            func(id<3>(x, y, z));
          }
        }
      }
      for (const accessor_info& arg : k->get_acc()) {
        arg.release_access();
      }
    };
#endif

  virtual void run(kernel k) {
    kernel::info_type ki = k.get_kernel_info();
    shared_ptr_class<kernel_info_cpu> kic =
        std::dynamic_pointer_cast<kernel_info_cpu>(ki);
    if (kic.get() == nullptr) {
      PRINT_ERR("invalid kernel_info: %lx", (size_t)ki.get());
      throw exception("set_capture() failed");
    }

    for (const detail::accessor_info& acc : k.get_acc()) {
      acc.acquire_access();
      alloc_mem(acc.container, acc.mode);
    }
    DEBUG_INFO("-- KENREL EXEC BEGIN --");
    try {
#ifdef DEBUG
      int ret_val = kic->func_();
#else
      kic->func_();
#endif
      DEBUG_INFO("-- KERNEL EXEC END (ret=%d) --", ret_val);
      // copy_out(ve_addr_list, k, proc);
    }
    catch (exception& e) {
      PRINT_ERR("kernel execution failed: %s", e.what());
      throw;
    }
    for (const detail::accessor_info& acc : k.get_acc()) {
      acc.release_access();
    }
  }

  // SUPPORT_PLATFORM_TYPE type() override { return CPU; }

  void* get_pointer(container_ptr p) override { return p->get_raw_ptr(); }
  void* alloc_mem(container_ptr p, access::mode = access::mode::read) override {
    return nullptr;
    // return p->get_raw_ptr();
  }
  void free_mem(container_ptr) override {}
  void copy_back() override {}

  void set_capture(kernel& k, void* p, size_t sz) {
    auto ki =
        std::dynamic_pointer_cast<detail::kernel_info_cpu>(k.get_kernel_info());
    if (ki && ki->capt_)
      std::memcpy(ki->capt_, p, sz);
  }

  void set_range(kernel& k, size_t r[6]) {
    auto ki =
        std::dynamic_pointer_cast<detail::kernel_info_cpu>(k.get_kernel_info());
    if (ki && ki->rnge_)
      std::memcpy(ki->rnge_, r, sizeof(size_t) * 6);
  }

  kernel create_kernel(const char* s) override {
    auto inf          = new detail::kernel_info_cpu(s);
    string_class capt = string_class("__") + s + "_obj__";
    string_class rnge = string_class("__") + s + "_range__";
    auto f            = dlsym(dll_, s);
    inf->func_        = reinterpret_cast<int (*)()>(f);
    inf->capt_        = dlsym(dll_, capt.c_str());
    inf->rnge_        = dlsym(dll_, rnge.c_str());
    if (!inf->func_) {
      PRINT_ERR("dlsym() failed: %s", s);
      throw exception("create_kernel() failed");
    }
    return kernel(inf);
  }
};

} // namespace neosycl::sycl::detail

#endif // NEOSYCL_INCLUDE_NEOSYCL_SYCL_TASK_HANDLER_HPP
