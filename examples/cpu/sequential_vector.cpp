#include <CL/sycl.hpp>

using namespace cl::sycl;

int main() {
  cl::sycl::queue q;

  T *a;
  T *b;
  T *c;

  buffer<T> buf_a(a, range<1>(n));
  buffer<T> buf_b(b, range<1>(n));
  buffer<T> buf_c(c, range<1>(n));

  q.submit([&](handler &cgh) {
    auto ka = buf_a.template get_access<access::mode::write>(cgh);
    auto kb = buf_b.template get_access<access::mode::read>(cgh);
    auto kc = buf_c.template get_access<access::mode::read>(cgh);
    cgh.parallel_for<class triad_kernel>(range<1>(n), [=](const id<1> &i) {
      ka[i] = kb[i] + 0.4 * kc[i];
    });
  });
  queue.wait();

  return 0;
}