
// Copyright (c) 2015-16 Tom Deakin, Simon McIntosh-Smith,
// University of Bristol HPC
//
// For full license terms please see the LICENSE file distributed with this
// source code

#include "SYCLStream.h"

#include <iostream>

using namespace cl::sycl;

REGISTER_KERNEL(copy_kernel);
REGISTER_KERNEL(mul_kernel);
REGISTER_KERNEL(add_kernel);
REGISTER_KERNEL(triad_kernel);
REGISTER_KERNEL(init_kernel);


template<class T>
SYCLStream<T>::SYCLStream(const unsigned int ARRAY_SIZE, const int device_index) {
  array_size = ARRAY_SIZE;
  const int VE_CORES = 8;

  queue = new cl::sycl::ve_queue();

  // Create buffers
  d_a = new buffer<T>(array_size);
  d_b = new buffer<T>(array_size);
  d_c = new buffer<T>(array_size);
  d_sum = new buffer<T>(array_size);
}

template<class T>
SYCLStream<T>::~SYCLStream() {
  delete d_a;
  delete d_b;
  delete d_c;
  delete d_sum;
  delete queue;
}

template<class T>
void SYCLStream<T>::copy() {
  queue->submit([&](handler &cgh) {
    auto ka = d_a->template get_access<access::mode::read>(cgh);
    auto kc = d_c->template get_access<access::mode::write>(cgh);
    cgh.parallel_for<copy_kernel>(range<1>{array_size}, [=](id<1> idx) {
      kc[idx] = ka[idx];
    });
  });
  queue->wait();
}

template<class T>
void SYCLStream<T>::mul() {
  queue->submit([&](handler &cgh) {
    auto kb = d_b->template get_access<access::mode::write>(cgh);
    auto kc = d_c->template get_access<access::mode::read>(cgh);
    cgh.parallel_for<mul_kernel>(range<1>{array_size}, [=](id<1> idx) {
      kb[idx] = 0.4 * kc[idx];
    });
  });
  queue->wait();
}

template<class T>
void SYCLStream<T>::add() {
  queue->submit([&](handler &cgh) {
    auto ka = d_a->template get_access<access::mode::read>(cgh);
    auto kb = d_b->template get_access<access::mode::read>(cgh);
    auto kc = d_c->template get_access<access::mode::write>(cgh);
    cgh.parallel_for<add_kernel>(range<1>{array_size}, [=](id<1> idx) {
      kc[idx] = ka[idx] + kb[idx];
    });
  });
  queue->wait();
}

template<class T>
void SYCLStream<T>::triad() {
  queue->submit([&](handler &cgh) {
    auto ka = d_a->template get_access<access::mode::write>(cgh);
    auto kb = d_b->template get_access<access::mode::read>(cgh);
    auto kc = d_c->template get_access<access::mode::read>(cgh);
    cgh.parallel_for<triad_kernel>(range<1>{array_size}, [=](id<1> idx) {
      ka[idx] = kb[idx] + 0.4 * kc[idx];
    });
  });
  queue->wait();
}

template<class T>
T SYCLStream<T>::dot() {
  return 0.0;
}

template<class T>
void SYCLStream<T>::init_arrays(T initA, T initB, T initC) {
  T a = initA;
  T b = initB;
  T c = initC;
  buffer<T, 1> init_a(&a, range<1>(1));
  buffer<T, 1> init_b(&b, range<1>(1));
  buffer<T, 1> init_c(&c, range<1>(1));

  queue->submit([&](handler &cgh) {
    auto ka = d_a->template get_access<access::mode::write>(cgh);
    auto kb = d_b->template get_access<access::mode::write>(cgh);
    auto kc = d_c->template get_access<access::mode::write>(cgh);
    auto init_a_val = init_a.template get_access<access::mode::read>(cgh);
    auto init_b_val = init_b.template get_access<access::mode::read>(cgh);
    auto init_c_val = init_c.template get_access<access::mode::read>(cgh);

    cgh.parallel_for<init_kernel>(range<1>{array_size}, [=](id<1> idx) {
      ka[idx] = init_a_val[0];
      kb[idx] = init_b_val[0];
      kc[idx] = init_c_val[0];
    });
  });
  queue->wait();
}

template<class T>
void SYCLStream<T>::read_arrays(std::vector<T> &a, std::vector<T> &b, std::vector<T> &c) {
  auto _a = d_a->template get_access<access::mode::read>();
  auto _b = d_b->template get_access<access::mode::read>();
  auto _c = d_c->template get_access<access::mode::read>();
  for (int i = 0; i < array_size; i++) {
    a[i] = _a[i];
    b[i] = _b[i];
    c[i] = _c[i];
  }
}

void listDevices(void) {
}

std::string getDeviceName(const int device) {
  return "";
}

std::string getDeviceDriver(const int device) {
  return "";
}

// TODO: Fix kernel names to allow multiple template specializations
template
class SYCLStream<float>;
template
class SYCLStream<double>;
