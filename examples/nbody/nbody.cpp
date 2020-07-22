#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <chrono>
#include <iostream>
#include <iomanip>
#include <sys/time.h>

#include <CL/sycl.hpp>

#define SOFTENING 1e-9f

REGISTER_KERNEL(bodyForce);
REGISTER_KERNEL(integratePos);

long get_time() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

using namespace cl::sycl;

void randomizeBodies(float *data, int n) {
  for (int i = 0; i < n; i++) {
    data[i] = 2.0f * (rand() / (float) RAND_MAX) - 1.0f;
  }
}

typedef struct {
  float x, y, z, vx, vy, vz;
} Body;

int main(const int argc, const char **argv) {
  ve_queue q;

  long nBodies = 30000;
  if (argc > 1) nBodies = atoi(argv[1]);

  const float dt = 0.01f; // time step
  const int nIters = 10;  // simulation iterations

  long totalTime = 0;
  buffer<long> n_buf(&nBodies, range<1>(1));

  for (int iter = 1; iter <= nIters; iter++) {
    long start = get_time();

    long bytes = nBodies * sizeof(Body);
    float *buf = (float *) malloc(bytes);
    Body *p = (Body *) buf;

    randomizeBodies(buf, 6 * nBodies); // Init pos / vel data

    buffer<Body> body_buf(p, range<1>(nBodies));

    q.submit([&](handler &cgh) {
      auto body_access = body_buf.get_access<access::mode::read_write>(cgh);
      auto n_bodys = n_buf.get_access<access::mode::read>(cgh);

      cgh.parallel_for<class bodyForce>(nBodies, [=](id<1> i) {
        const float dt = 0.01f;
        const long nBodies = n_bodys[0];

        float Fx = 0.0f;
        float Fy = 0.0f;
        float Fz = 0.0f;

        for (int j = 0; j < nBodies; j++) {
          float dx = body_access[j].x - body_access[i].x;
          float dy = body_access[j].y - body_access[i].y;
          float dz = body_access[j].z - body_access[i].z;
          float distSqr = dx * dx + dy * dy + dz * dz + 1e-9f;
          float invDist = 1.0f / sqrtf(distSqr);
          float invDist3 = invDist * invDist * invDist;

          Fx += dx * invDist3;
          Fy += dy * invDist3;
          Fz += dz * invDist3;
        }

        body_access[i].vx += dt * Fx;
        body_access[i].vy += dt * Fy;
        body_access[i].vz += dt * Fz;
      });
    });

//    q.wait();

    q.submit([&](handler &cgh) {
      auto body_access = body_buf.get_access<access::mode::read_write>(cgh);

      cgh.parallel_for<class integratePos>(nBodies, [=](id<1> i) {
        const float dt = 0.01f;

        body_access[i].x += body_access[i].vx * dt;
        body_access[i].y += body_access[i].vy * dt;
        body_access[i].z += body_access[i].vz * dt;
      });
    });

//    q.wait();

    long end = get_time();
    long cost = end - start;

    if (iter > 1) { // First iter is warm up
      totalTime += cost;
    }
    free(buf);
    printf("Iteration %d: %ld ms\n", iter, cost);
  }

  double avgTime = (double) totalTime / (double) (nIters - 1);

  printf("%ld Bodies: All time %ld ms ,Avg time: %0.3f ms\n", nBodies, totalTime, avgTime);

}