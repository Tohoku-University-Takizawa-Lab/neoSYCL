#include <math.h>
#include <stdio.h>

int add_kernel(double *ka, double *kb, double *kc, int N, int STEP) {
  for (int i = 0; i < N; i++) {
    kc[i] = ka[i] + kb[i];
  }
  return 0;
}
int copy_kernel(double *ka, double *kc, int N, int STEP) {
  for (int i = 0; i < N; i++) {
    kc[i] = ka[i];
  }
  return 0;
}
int dot_kernel(double *ka, double *kb, double *k_ret, int N, int STEP) {
  for (int i = 0; i < N; i++) {
    k_ret[0] += ka[i] * kb[i];
  }
  return 0;
}

int mul_kernel(double *kb, double *kc, int N, int STEP) {
  for (int i = 0; i < N; i++) {
    kb[i] = 0.4 * kc[i];
  }
  return 0;
}
int triad_kernel(double *ka, double *kb, double *kc, int N, int STEP) {
  for (int i = 0; i < N; i++) {
    ka[i] = kb[i] + 0.4 * kc[i];
  }
  return 0;
}
