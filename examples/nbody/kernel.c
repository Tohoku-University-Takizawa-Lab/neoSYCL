#include <math.h>
#include <stdio.h>

typedef struct {
  float x, y, z, vx, vy, vz;
} Body;

int bodyForce(Body *body_access, long *n_bodys, int N, int STEP) {
  for (int i = 0; i < N; i += 1) {
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
  }
  return 0;
}
int integratePos(Body *body_access, int N, int STEP) {
  for (int i = 0; i < N; i += 1) {
    const float dt = 0.01f;

    body_access[i].x += body_access[i].vx * dt;
    body_access[i].y += body_access[i].vy * dt;
    body_access[i].z += body_access[i].vz * dt;
  }
  return 0;
}
