
#include "SYCLStream.h"

REGISTER_KERNEL(init_kernel);
REGISTER_KERNEL(read_kernel);
REGISTER_KERNEL(copy_kernel);
REGISTER_KERNEL(mul_kernel);
REGISTER_KERNEL(add_kernel);
REGISTER_KERNEL(triad_kernel);
REGISTER_KERNEL(dot_kernel);

template<class T>
SYCLStream<T>::SYCLStream(const unsigned int ARRAY_SIZE, int device):queue() {
  array_size = ARRAY_SIZE;

  this->a = (T *) malloc(sizeof(T) * array_size);
  this->b = (T *) malloc(sizeof(T) * array_size);
  this->c = (T *) malloc(sizeof(T) * array_size);
}

template<class T>
SYCLStream<T>::~SYCLStream() {
  free(a);
  free(b);
  free(c);
}

template<class T>
void SYCLStream<T>::init_arrays(T initA, T initB, T initC) {
  unsigned int array_size = this->array_size;

  for (int i = 0; i < array_size; i++) {
    a[i] = initA;
    b[i] = initB;
    c[i] = initC;
  }
}

template<class T>
void SYCLStream<T>::read_arrays(std::vector<T> &h_a, std::vector<T> &h_b, std::vector<T> &h_c) {
  for (int i = 0; i < array_size; i++) {
    h_a[i] = a[i];
    h_b[i] = b[i];
    h_c[i] = c[i];
  }
}

template<class T>
void SYCLStream<T>::copy() {
  buffer<T> buf_a(a, range<1>(array_size));
  buffer<T> buf_c(c, range<1>(array_size));

  queue.submit([&](handler &cgh) {
    auto ka = buf_a.template get_access<access::mode::read>(cgh);
    auto kc = buf_c.template get_access<access::mode::write>(cgh);
    cgh.parallel_for<class copy_kernel>(range<1>{array_size}, [=](const id<1> &i) {
      kc[i] = ka[i];
    });
  });
  queue.wait();
}

template<class T>
void SYCLStream<T>::mul() {
  buffer<T> buf_b(b, range<1>(array_size));
  buffer<T> buf_c(c, range<1>(array_size));

  queue.submit([&](handler &cgh) {
    auto kb = buf_b.template get_access<access::mode::write>(cgh);
    auto kc = buf_c.template get_access<access::mode::read>(cgh);
    cgh.parallel_for<class mul_kernel>(range<1>{array_size}, [=](const id<1> &i) {
      kb[i] = 0.4 * kc[i];
    });
  });
  queue.wait();
}

template<class T>
void SYCLStream<T>::add() {
  buffer<T> buf_a(a, range<1>(array_size));
  buffer<T> buf_b(b, range<1>(array_size));
  buffer<T> buf_c(c, range<1>(array_size));

  queue.submit([&](handler &cgh) {
    auto ka = buf_a.template get_access<access::mode::read>(cgh);
    auto kb = buf_b.template get_access<access::mode::read>(cgh);
    auto kc = buf_c.template get_access<access::mode::write>(cgh);
    cgh.parallel_for<class add_kernel>(range<1>{array_size}, [=](const id<1> &i) {
      kc[i] = ka[i] + kb[i];
    });
  });
  queue.wait();
}

template<class T>
void SYCLStream<T>::triad() {
  buffer<T> buf_a(a, range<1>(array_size));
  buffer<T> buf_b(b, range<1>(array_size));
  buffer<T> buf_c(c, range<1>(array_size));

  queue.submit([&](handler &cgh) {
    auto ka = buf_a.template get_access<access::mode::write>(cgh);
    auto kb = buf_b.template get_access<access::mode::read>(cgh);
    auto kc = buf_c.template get_access<access::mode::read>(cgh);
    cgh.parallel_for<class triad_kernel>(range<1>(array_size), [=](const id<1> &i) {
      ka[i] = kb[i] + 0.4 * kc[i];
    });
  });
  queue.wait();
}

template<class T>
T SYCLStream<T>::dot() {
  T ret;

  buffer<T> ret_buf(&ret, range<1>(1));

  buffer<T> buf_a(a, range<1>(array_size));
  buffer<T> buf_b(b, range<1>(array_size));

  queue.submit([&](handler &cgh) {
    auto ka = buf_a.template get_access<access::mode::read>(cgh);
    auto kb = buf_b.template get_access<access::mode::read>(cgh);
    auto k_ret = ret_buf.template get_access<access::mode::write>(cgh);
    cgh.parallel_for<class dot_kernel>(range<1>{array_size}, [=](const id<1> &i) {
      k_ret[0] += ka[i] * kb[i];
    });
  });
  queue.wait();
  return ret;
}

void listDevices(void) {
  std::cout << "0: VE" << std::endl;
}

std::string getDeviceName(const int) {
  return std::string("Device name unavailable");
}

std::string getDeviceDriver(const int) {
  return std::string("Device driver unavailable");
}
template
class SYCLStream<double>;

template
class SYCLStream<float>;
