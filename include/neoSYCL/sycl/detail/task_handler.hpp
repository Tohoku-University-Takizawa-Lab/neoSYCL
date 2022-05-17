#ifndef NEOSYCL_INCLUDE_NEOSYCL_SYCL_TASK_HANDLER_HPP
#define NEOSYCL_INCLUDE_NEOSYCL_SYCL_TASK_HANDLER_HPP

#include "neoSYCL/sycl/kernel.hpp"

namespace neosycl::sycl::detail {

  class task_handler {
  protected:
    using container_type   = shared_ptr_class<detail::container::DataContainer>;
    using kernel_info_type = kernel::info_type;

  public:
    explicit task_handler() {}

    virtual void single_task(shared_ptr_class<kernel> k,
      const std::function<void(void)>& func) = 0;

    virtual void parallel_for_1d(shared_ptr_class<kernel> k, range<1> r,
      const std::function<void(id<1>)>& func,
      id<1> offset) = 0;

    virtual void parallel_for_2d(shared_ptr_class<kernel> k, range<2> r,
      const std::function<void(id<2>)>& func,
      id<2> offset) = 0;

    virtual void parallel_for_3d(shared_ptr_class<kernel> k, range<3> r,
      const std::function<void(id<3>)>& func,
      id<3> offset) = 0;

    virtual SUPPORT_PLATFORM_TYPE type() = 0;

    virtual void* get_pointer(container_type)                  = 0;
    virtual void* alloc_mem(container_type,
                            access::mode = access::mode::read) = 0;
    virtual void free_mem(container_type)                      = 0;
    virtual void copy_back()                                   = 0;

    virtual void set_capture(shared_ptr_class<kernel>, void* p, size_t sz) {}
    virtual void set_range(shared_ptr_class<kernel>, size_t r[6]) {}

    template <size_t dim>
    void set_range(shared_ptr_class<kernel> k, range<dim> r) {
      size_t sz[6] = {1, 1, 1, 0, 0, 0};
      for (size_t idx(0); idx != dim; idx++) {
        sz[idx] = r[idx];
      }
      set_range(k, sz);
    }

    template <size_t dim>
    void set_range(shared_ptr_class<kernel> k, range<dim> r, id<dim> i) {
      size_t sz[6] = {1, 1, 1, 0, 0, 0};
      for (size_t idx(0); idx != dim; idx++) {
        sz[idx] = r[idx];
      }
      for (size_t idx(3); idx != dim + 3; idx++) {
        sz[idx] = i[idx];
      }
      set_range(k, sz);
    }

    virtual kernel* create_kernel(const char*) = 0;
  };

  class task_handler_cpu : public task_handler {
  public:
    task_handler_cpu() {}

    void single_task(shared_ptr_class<kernel> k,
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

    void parallel_for_1d(shared_ptr_class<kernel> k, range<1> r,
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

    void parallel_for_2d(shared_ptr_class<kernel> k, range<2> r,
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

    void parallel_for_3d(shared_ptr_class<kernel> k, range<3> r,
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

    SUPPORT_PLATFORM_TYPE type() override { return CPU; }

    void* get_pointer(container_type p) override { return p->get_raw_ptr(); }
    void* alloc_mem(container_type p, access::mode = access::mode::read) override {
      return nullptr;
      //return p->get_raw_ptr();
    }
    void free_mem(container_type) override {}
    void copy_back() override {}

    kernel* create_kernel(const char* s) override {
      return new kernel(new detail::kernel_info_cpu(s));
    }
  };

} // namespace neosycl::sycl::detail

#endif // NEOSYCL_INCLUDE_NEOSYCL_SYCL_TASK_HANDLER_HPP
