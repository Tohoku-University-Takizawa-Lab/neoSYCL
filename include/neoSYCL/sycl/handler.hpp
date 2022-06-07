#pragma once
#include "neoSYCL/sycl/detail/task_counter.hpp"
#include "neoSYCL/sycl/detail/handler.hpp"
#include "neoSYCL/sycl/detail/accessor_data.hpp"

namespace neosycl::sycl {

///////////////////////////////////////////////////////////////////////////////
class handler {
  using counter_type = shared_ptr_class<detail::task_counter>;
  using handler_type = shared_ptr_class<detail::program_data>;

  friend class queue;

  explicit handler(device d, program p, counter_type counter)
      : dev_(std::move(d)), prog_(std::move(p)), cntr_(std::move(counter)),
        hndl_(prog_.get_data(dev_)) {}

  ~handler() {
    for (size_t i(0); i < acc_.size(); i++) {
      // DEBUG_INFO("memory unlock: %p", acc_[i].data.get());
      if (acc_[i].mode != access::mode::read)
        acc_[i].data->unlock_write();
      else
        acc_[i].data->unlock_read();
    }
  }

public:
  template <typename KernelName, typename KernelType, int dimensions>
  void run(range<dimensions> kernelRange, id<dimensions> kernelOffset,
           KernelType kernelFunc) {
    kernel k = prog_.get_kernel<KernelName>();
    hndl_->set_range(k, kernelRange, kernelOffset);

    auto [ptr, sz] = kernelFunc();
    // DEBUG_INFO("kernel %s %p %lu", k.get_name(),ptr,sz);
    hndl_->set_capture(k, ptr, sz);
    hndl_->run(k);
  }

  template <typename KernelName, typename KernelType, int dimensions>
  void run(range<dimensions> kernelRange, KernelType kernelFunc) {
    kernel k = prog_.get_kernel<KernelName>();
    hndl_->set_range(k, kernelRange);

    auto [ptr, sz] = kernelFunc();
    // DEBUG_INFO("kernel %s %p %lu", k.get_name(),ptr,sz);
    hndl_->set_capture(k, ptr, sz);
    hndl_->run(k);
  }

  template <typename KernelName, typename KernelType>
  void run(KernelType kernelFunc) {
    kernel k = prog_.get_kernel<KernelName>();

    auto [ptr, sz] = kernelFunc();
    // DEBUG_INFO("kernel %s %p %lu", k.get_name(),ptr,sz);
    hndl_->set_capture(k, ptr, sz);
    hndl_->run(k);
  }

  template <typename KernelName, typename KernelType>
  void single_task(KernelType kernelFunc) {
    if (!dev_.is_host())
      return;
    detail::single_task(kernelFunc);
  }

  template <typename KernelName, typename KernelType, int dimensions>
  void parallel_for(range<dimensions> numWorkItems, KernelType kernelFunc) {
    if (!dev_.is_host())
      return;
    detail::parallel_for(numWorkItems, kernelFunc, id<dimensions>{},
                         get_index_type(kernelFunc));
  }

  template <typename KernelName, typename KernelType, int dimensions>
  void parallel_for(range<dimensions> numWorkItems,
                    id<dimensions> workItemOffset, KernelType kernelFunc) {
    if (!dev_.is_host())
      return;
    detail::parallel_for(numWorkItems, kernelFunc, workItemOffset,
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
    /* allocate and lock a device buffer for this placeholder */
    this->alloc_mem_(acc);
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
  }

  template <typename T, int D, access::mode m, access::target t,
            access::placeholder p>
  void* alloc_mem_(accessor<T, D, m, t, p>& acc) {
    using container_type  = typename accessor<T, D, m, t, p>::container_type;
    using device_ptr_type = detail::container::device_ptr_type;
    shared_ptr_class<container_type> buf = acc.data;

    // allocate a device memory chunk if not allocated yet
    int count = 0;
    if (dev_.is_host() == false) {
      count = buf->map.count(hndl_);
      if (count == 1)
        // multiple accessors would use the same buffer
        acc.device_ptr = buf->map.at(hndl_).ptr;
      else if (count == 0) {
        void* dp = hndl_->alloc_mem(buf->get_raw_ptr(), buf->get_size(), m);
        device_ptr_type cdp = {dp, m};
        buf->map.insert(std::make_pair(hndl_, cdp));
        acc.device_ptr = dp;
      }
      else
        throw runtime_error("invalid BufferContainer object");
      // DEBUG_INFO("device ptr %p", acc.device_ptr);
    }

    // check if the buffer is already locked
    size_t i = 0;
    if (count > 0) {
      // (count > 0) does not mean it is already locked by this handler.
      // so let's check if it has been locked so far.
      for (i = 0; i < acc_.size(); i++)
        if (acc_[i].data.get() == acc.data.get()) {
          if (acc_[i].mode == access::mode::read && m != access::mode::read) {
            // not sure if this is a thread-safe way...
            acc_[i].data->unlock_read();
            acc_[i].data->lock_write();
            acc_[i].mode            = m; // this is used for unlocking
            buf->map.at(hndl_).mode = m; // this is used for buffer copy back
          }
          break;
        }
    }
    // lock the buffer because it's not locked by this handler
    if (i == acc_.size()) {
      // DEBUG_INFO("memory lock: %p", acc.data.get());
      acc_.push_back(detail::accessor_data(acc.data, m));
      if (m != access::mode::read)
        acc.data->lock_write();
      else
        acc.data->lock_read();
    }

    if (dev_.is_host())
      return acc.data->get_raw_ptr();
    return acc.device_ptr;
  }

  template <typename T, int D, access::mode m, access::target t,
            access::placeholder p>
  neosycl::sycl::rt::acc_<T> map_(sycl::accessor<T, D, m, t, p> acc) {
    size_t sz[6] = {1, 1, 1, 0, 0, 0};
    std::memcpy(sz + 0, &acc.get_range()[0], sizeof(size_t) * D);
    std::memcpy(sz + 3, &acc.get_offset()[0], sizeof(size_t) * D);
    return neosycl::sycl::rt::acc_<T>{
        get_ptr_(acc), {sz[0], sz[1], sz[2], sz[3], sz[4], sz[5]}};
  }

private:
  device dev_;
  program prog_;
  counter_type cntr_;
  handler_type hndl_;
  vector_class<detail::accessor_data> acc_;

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
