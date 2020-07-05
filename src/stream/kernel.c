#include <math.h>
#include <stdio.h>
#include <stdint.h>

int copy_kernel(double *ka, double *kc, int N, int STEP) {
  for (size_t idx = 0; idx != N; idx += STEP) {
    kc[idx] = ka[idx];
  }
  return 233;
}

int mul_kernel(double *kb, double *kc, int N, int STEP) {
  for (size_t idx = 0; idx != N; idx += STEP) {
    kb[idx] = 0.4 * kc[idx];
  }
  return 233;
}

int add_kernel(double *ka, double *kb, double *kc, int N, int STEP) {
  for (size_t idx = 0; idx != N; idx += STEP) {
    kc[idx] = ka[idx] + kb[idx];
  }
  return 233;
}

int triad_kernel(double *ka, double *kb, double *kc, int N, int STEP) {
  for (size_t idx = 0; idx != N; idx += STEP) {
    ka[idx] = kb[idx] + 0.4 * kc[idx];
  }
  return 233;
}

int init_kernel(double *ka,
                 double *kb,
                 double *kc,
                 double *init_a_val,
                 double *init_b_val,
                 double *init_c_val,
                 int N,
                 int STEP) {
  for (size_t idx = 0; idx != N; idx += STEP) {
    ka[idx] = init_a_val[0];
    kb[idx] = init_b_val[0];
    kc[idx] = init_c_val[0];
  }
  return 233;
}




