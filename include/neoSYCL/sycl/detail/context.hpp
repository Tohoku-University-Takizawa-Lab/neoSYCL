#pragma once

namespace neosycl::sycl {
namespace detail {

class context_impl {
public:
  context_impl(context c) : ctx_(c) {}
  ~context_impl() = default;

  context ctx_;
  vector_class<device> dev_;
};
} // namespace detail

vector_class<device> context::get_devices() const { return impl_->dev_; }

void context::init(vector_class<device> dev) {
  impl_ = nullptr;
  if (dev.size() == 0) {
    DEBUG_INFO("context created with no device");
    return;
  }
  impl_ =
      shared_ptr_class<detail::context_impl>(new detail::context_impl(*this));
  for (auto& d : dev)
    if (d.get_impl() == nullptr)
      throw invalid_object_error("null device object found");
  impl_->dev_ = dev;
}
#if 0
void context::free_mem_(container_ptr c) {
  if (!impl_)
    return;
  for (auto& d : impl_->dev_)
    d.impl_->free_mem(c);
}
#endif
} // namespace neosycl::sycl
