#include <math.h>
#include <stdio.h>
#include <stdint.h>

void sequential_vector(float *ka, float *kb, float *kc) {
  for (size_t i = 0; i != 3; i++) {
    kc[i] = ka[i] + kb[i];
  }
}

