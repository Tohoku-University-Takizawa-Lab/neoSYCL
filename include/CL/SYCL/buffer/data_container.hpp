#ifndef SYCL_INCLUDE_CL_SYCL_BUFFER_DATA_CONTAINER_HPP_
#define SYCL_INCLUDE_CL_SYCL_BUFFER_DATA_CONTAINER_HPP_

namespace cl::sycl::detail {

// Data container interface
class DataContainer {
 private:
  mutable std::shared_mutex mtx;

 public:

  void lock_read() const {
    mtx.lock_shared();
  }

  void unlock_read() const {
    mtx.unlock_shared();
  }

  void lock_write() const {
    mtx.lock();
  }

  void unlock_write() const {
    mtx.unlock();
  }

  virtual void *get_data_ptr() = 0;

  virtual size_t get_size() = 0;

};

}

#endif //SYCL_INCLUDE_CL_SYCL_BUFFER_DATA_CONTAINER_HPP_
