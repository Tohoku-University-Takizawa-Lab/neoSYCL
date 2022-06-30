#pragma once

namespace neosycl::sycl {
namespace detail {

template <info::context param>
typename info::param_traits<info::context, param>::return_type
get_info(context_impl*);

///////////////////////////////////////////////////////////////////////////////
class context_impl {
public:
  context_impl(context c) : ctx_(c) {}
  ~context_impl() = default;

  context ctx_;
  vector_class<device> dev_;

  template <info::context param>
  typename info::param_traits<info::context, param>::return_type get_info() {
    return get_info<param>(this);
  }
};

template <>
typename info::param_traits<info::context,
                            info::context::reference_count>::return_type
get_info<info::context::reference_count>(context_impl* c) {
  return c->ctx_.use_count_();
}
template <>
typename info::param_traits<info::context, info::context::platform>::return_type
get_info<info::context::platform>(context_impl* c) {
  return c->dev_[0].get_platform();
}
template <>
typename info::param_traits<info::context, info::context::devices>::return_type
get_info<info::context::devices>(context_impl* c) {
  return c->dev_;
}
} // namespace detail

vector_class<device> context::get_devices() const {
  return impl_->dev_;
}

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

template <info::context param>
typename info::param_traits<info::context, param>::return_type
context::get_info() const {
  return impl_->get_info<param>();
}

} // namespace neosycl::sycl
