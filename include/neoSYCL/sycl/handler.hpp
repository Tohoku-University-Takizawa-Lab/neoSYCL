#pragma once
#include "neoSYCL/sycl/detail/task_counter.hpp"
#include "neoSYCL/sycl/detail/handler.hpp"

namespace neosycl::sycl {

///////////////////////////////////////////////////////////////////////////////
class handler {
  using counter_type = shared_ptr_class<detail::task_counter>;
  using handler_type = shared_ptr_class<detail::program_data>;

  friend class queue;
  explicit handler(context c, device d, program p, counter_type counter)
      : ctx_(c), dev_(d), prog_(p), cntr_(std::move(counter)), kernel_() {
    hndl_ = prog_.get_data(dev_);
  }

public:
  template <typename KernelName, typename KernelType, int dimensions>
  void run(range<dimensions> kernelRange, id<dimensions> kernelOffset,
           KernelType kernelFunc) {
    kernel_ = prog_.get_kernel<KernelName>();
    kernel_.get_acc().clear();
    hndl_->set_range(kernel_, kernelRange, kernelOffset);

    kernelFunc();

    // hndl_->run(kernel_);
    submit_task([h = hndl_, k = kernel_]() { h->run(k); });
  }

  template <typename KernelName, typename KernelType, int dimensions>
  void run(range<dimensions> kernelRange, KernelType kernelFunc) {
    kernel_ = prog_.get_kernel<KernelName>();
    kernel_.get_acc().clear();
    hndl_->set_range(kernel_, kernelRange);

    kernelFunc();

    // hndl_->run(kernel_);
    submit_task([h = hndl_, k = kernel_]() { h->run(k); });
  }

  template <typename KernelName, typename KernelType>
  void run(KernelType kernelFunc) {
    kernel_ = prog_.get_kernel<KernelName>();
    kernel_.get_acc().clear();

    kernelFunc();

    // hndl_->run(kernel_);
    submit_task([h = hndl_, k = kernel_]() { h->run(k); });
  }

  template <typename KernelName>
  void copy_capture(KernelName* p) {
    hndl_->set_capture(kernel_, p, sizeof(KernelName));
  }

  template <typename KernelName, typename KernelType>
  void single_task(KernelType kernelFunc) {
    if (!dev_.is_host())
      return;
    detail::single_task(kernel_, kernelFunc);
  }

  template <typename KernelName, typename KernelType, int dimensions>
  void parallel_for(range<dimensions> numWorkItems, KernelType kernelFunc) {
    if (!dev_.is_host())
      return;
    detail::parallel_for(kernel_, numWorkItems, kernelFunc, id<dimensions>{},
                         get_index_type(kernelFunc));
  }

  template <typename KernelName, typename KernelType, int dimensions>
  void parallel_for(range<dimensions> numWorkItems,
                    id<dimensions> workItemOffset, KernelType kernelFunc) {
    if (!dev_.is_host())
      return;
    detail::parallel_for(kernel_, numWorkItems, kernelFunc, workItemOffset,
                         get_index_type(kernelFunc));
  }

  // experimental impl just for translation testing
  template <typename KernelName, typename KernelType, int dimensions>
  void parallel_for(nd_range<dimensions> executionRange,
                    KernelType kernelFunc) {
    PRINT_ERR("Not implemented yet.")
    throw unimplemented();
  }

  template <typename KernelName, typename WorkgroupFunctionType, int dimensions>
  void parallel_for_work_group(range<dimensions> numWorkGroups,
                               WorkgroupFunctionType kernelFunc) {
    PRINT_ERR("Not implemented yet.")
    throw unimplemented();
  }

  template <typename KernelName, typename WorkgroupFunctionType, int dimensions>
  void parallel_for_work_group(range<dimensions> numWorkGroups,
                               range<dimensions> workGroupSize,
                               WorkgroupFunctionType kernelFunc) {
    PRINT_ERR("Not implemented yet.")
    throw unimplemented();
    ;
  }

  //----- OpenCL interoperability interface //
  template <typename T>
  void set_arg(int argIndex, T&& arg);

  template <typename... Ts>
  void set_args(Ts&&... args);

  template <typename T, int D, access::mode m, access::target t>
  void require(sycl::accessor<T, D, m, t, access::placeholder::true_t> acc) {
    hndl_->alloc_mem(*acc.data, m);
    // -- need something but not implemented yet
    // acc_.push_back(detail::accessor_data(acc, m));
  }

  //------ Explicit memory operation APIs   //
  template <typename T_src, int dim_src, access::mode mode_src,
            access::target tgt_src, access::placeholder isPlaceholder,
            typename T_dst>
  void copy(accessor<T_src, dim_src, mode_src, tgt_src, isPlaceholder> src,
            shared_ptr_class<T_dst> dst) {
    DEBUG_INFO("handler::copy(%p, %p, %lu)", get_ptr_(src), dst.get(),
               src.data->get_size());
    hndl_->read_mem((void*)dst.get(), get_ptr_(src), src.data->get_size());
  }

  template <typename T_src, typename T_dst, int dim_dst, access::mode mode_dst,
            access::target tgt_dst, access ::placeholder isPlaceholder>
  void copy(shared_ptr_class<T_src> src,
            accessor<T_dst, dim_dst, mode_dst, tgt_dst, isPlaceholder> dst) {
    DEBUG_INFO("handler::copy(%p, %p, %lu)", src.get(), get_ptr_(dst),
               dst.data->get_size());
    hndl_->write_mem(get_ptr_(dst), (void*)src.get(), dst.data->get_size());
  }

  template <typename T_src, int dim_src, access::mode mode_src,
            access::target tgt_src, access::placeholder isPlaceholder,
            typename T_dst>
  void copy(accessor<T_src, dim_src, mode_src, tgt_src, isPlaceholder> src,
            T_dst* dst) {
    DEBUG_INFO("handler::copy(%p, %p, %lu)", get_ptr_(src), dst,
               src.data->get_size());
    hndl_->read_mem((void*)dst, get_ptr_(src), src.data->get_size());
  }

  template <typename T_src, typename T_dst, int dim_dst, access::mode mode_dst,
            access::target tgt_dst, access ::placeholder isPlaceholder>
  void copy(const T_src* src,
            accessor<T_dst, dim_dst, mode_dst, tgt_dst, isPlaceholder> dst) {
    DEBUG_INFO("handler::copy(%p, %p, %lu)", src, get_ptr_(dst),
               dst.data->get_size());
    hndl_->write_mem(get_ptr_(dst), (void*)src, dst.data->get_size());
  }

  template <typename T_src, int dim_src, access::mode mode_src,
            access::target tgt_src, access::placeholder isPlaceholder_src,
            typename T_dst, int dim_dst, access::mode mode_dst,
            access::target tgt_dst, access ::placeholder isPlaceholder_dst>
  void
  copy(accessor<T_src, dim_src, mode_src, tgt_src, isPlaceholder_src> src,
       accessor<T_dst, dim_dst, mode_dst, tgt_dst, isPlaceholder_dst> dst) {
    DEBUG_INFO("handler::copy(%p, %p, %lu)", get_ptr_(src), get_ptr_(dst),
               dst.data->get_size());
    hndl_->copy_mem(get_ptr_(dst), get_ptr_(src), dst.data->get_size());
  }

  template <typename T, int dim, access::mode mode, access::target tgt,
            access::placeholder isPlaceholder>
  void update_host(accessor<T, dim, mode, tgt, isPlaceholder> acc) {
    DEBUG_INFO("handler::update_host(%p)", get_ptr_(acc));
    hndl_->read_mem(acc.data->get_raw_ptr(), (void*)get_ptr_(acc),
                    acc.data->get_size());
  }

  template <typename T, int dim, access::mode mode, access::target tgt,
            access::placeholder isPlaceholder>
  void fill(accessor<T, dim, mode, tgt, isPlaceholder> dst, const T& src) {
    DEBUG_INFO("handler::fill(%p)", get_ptr_(dst));
    shared_ptr_class<T> tmp(new T[dst.get_count()]);
    std::fill_n((T*)tmp.get(), dst.get_count(), src);
    hndl_->write_mem((void*)get_ptr_(dst), (void*)tmp.get(),
                     dst.data->get_size());
  }

  //----- INTERNAL USE ONLY --------------- //
  template <typename T, int D, access::mode m, access::target t,
            access::placeholder p>
  T* get_ptr_(sycl::accessor<T, D, m, t, p> acc) {
    if (acc.device_ptr)
      return (T*)acc.device_ptr;

    using container_type = typename accessor<T, D, m, t, p>::container_type;
    shared_ptr_class<container_type> buf = acc.data;

    if (dev_.is_host())
      return (T*)buf->get_raw_ptr();
    else if (buf->map.count(hndl_))
      return (T*)buf->map.at(hndl_).ptr;
    throw runtime_error("invalid BufferContainer object");
    // return (T*)hndl_->get_ptr_(*acc.data);
  }

  template <typename T, int D, access::mode m, access::target t,
            access::placeholder p>
  void* alloc_mem_(accessor<T, D, m, t, p>& acc) {
    if (dev_.is_host())
      return acc.data->get_raw_ptr();

    using container_type = typename accessor<T, D, m, t, p>::container_type;
    using device_ptr     = typename container_type::device_ptr;
    shared_ptr_class<container_type> buf = acc.data;

    int count = buf->map.count(hndl_);
    if (count == 1)
      // multiple accessors use the same buffer
      acc.device_ptr = buf->map.at(hndl_).ptr;
    else if (count == 0) {
      void* dp       = hndl_->alloc_mem(buf->get_raw_ptr(), buf->get_size(), m);
      device_ptr cdp = {dp, m};
      buf->map.insert(std::make_pair(hndl_, cdp));
      acc.device_ptr = dp;
    }
    else
      throw runtime_error("invalid BufferContainer object");
    DEBUG_INFO("device ptr %p", acc.device_ptr);
    // acc.device_ptr = hndl_->alloc_mem(*acc.data, m);
    //  avoid deleting program_data before buffer destruction.
    // acc.data->progs.push_back(prog_);
    return acc.device_ptr;
  }

  // context& get_context() {
  //   return ctx_;
  // }

  // accessor_list& get_acc_() { return acc_; }

  template <typename T, int D, access::mode m, access::target t,
            access::placeholder p>
  neosycl::sycl::rt::acc_<T> map_(sycl::accessor<T, D, m, t, p> acc) {
    size_t sz[6] = {1, 1, 1, 0, 0, 0};
    size_t i;
    for (i = 0; i < kernel_.get_acc().size(); i++)
      if (kernel_.get_acc()[i].data.get() == acc.data.get()) {
        if (m != access::mode::read)
          kernel_.get_acc()[i].mode = m;
        break;
      }
    if (i == kernel_.get_acc().size()) {
      kernel_.get_acc().push_back(detail::accessor_data(acc.data, m));
    }
    std::memcpy(sz + 0, &acc.get_range()[0], sizeof(size_t) * D);
    std::memcpy(sz + 3, &acc.get_offset()[0], sizeof(size_t) * D);
    return neosycl::sycl::rt::acc_<T>{
        get_ptr_(acc), {sz[0], sz[1], sz[2], sz[3], sz[4], sz[5]}};
  }

private:
  context ctx_;
  device dev_;
  program prog_;
  counter_type cntr_;
  kernel kernel_;
  handler_type hndl_;

  template <typename Func>
  void submit_task(Func func) {
    cntr_->incr();
    std::thread t([f = func, c = cntr_]() {
      try {
        f();
      }
      catch (...) {
        throw;
      }
      c->decr();
    });
    t.detach();
  }

  template <typename F, typename retT, typename argT>
  auto index_type_ptr(retT (F::*)(argT)) {
    return (argT*)nullptr;
  }

  template <typename F, typename retT, typename argT>
  auto index_type_ptr_(retT (F::*)(argT) const) {
    return (argT*)nullptr;
  }

  template <typename KernelType>
  auto get_index_type(const KernelType&) {
    return index_type_ptr_(&KernelType::operator());
  }
};

// called by accessor::accessor()
template <typename T, int D, access::mode m, access::target t,
          access::placeholder p>
void accessor<T, D, m, t, p>::alloc_(handler& h) {
  h.alloc_mem_(*this);
}
} // namespace neosycl::sycl
