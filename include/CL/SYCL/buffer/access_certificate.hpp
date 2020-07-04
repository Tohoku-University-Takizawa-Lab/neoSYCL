#ifndef SYCL_INCLUDE_CL_SYCL_BUFFER_ACCESS_CERTIFICATE_HPP_
#define SYCL_INCLUDE_CL_SYCL_BUFFER_ACCESS_CERTIFICATE_HPP_


namespace cl::sycl::detail {

class AccessCertificate {
 private:
  std::function<void(void)> callback;

 public:
//  AccessCertificate(const buffer<T, Dimensions> &buf, detail::DataContainerND<T, Dimensions> *data)
//      : buf(buf), data(data) {
//    if (mode == access::mode::read_write || mode == access::mode::write) {
//      buf.lock_write();
//    } else {
//      buf.lock_read();
//    }
//  }

  AccessCertificate(const std::function<void(void)> &callback) : callback(callback) {}

  virtual ~AccessCertificate() {
    callback();
  }

};
}

#endif //SYCL_INCLUDE_CL_SYCL_BUFFER_ACCESS_CERTIFICATE_HPP_
