#include "raytracer/Perlin.h"
#include "raytracer/random_numbers.h"

namespace {

float lerp(float a, float b, float t) {
  return (1.0f - t) * a + t * b;
}

float trilerp(float c[2][2][2], float u, float v, float w) {
  float accum = 0;
  for (int i=0; i<2; i++) {
    for (int j=0; j<2; j++) {
      for (int k=0; k<2; k++) {
        accum += 
          (i * u + (1.0f - i) * (1.0f - u)) *
          (j * v + (1.0f - j) * (1.0f - v)) *
          (k * w + (1.0f - k) * (1.0f - w)) *
          c[i][j][k];
      }
    }
  }

  return accum;
}

} // internal namespace

Perlin::Perlin() {
  _data = generate();
  _permutation_x = generate_permutation();
  _permutation_y = generate_permutation();
  _permutation_z = generate_permutation();
}

Perlin::data_t Perlin::generate() {
  Perlin::data_t data;
  for (size_t i=0; i < data.size(); ++i) {
    data[i] = random_number();
  }
  return data;
}

void Perlin::permute(permutation_t &p) {
  for (int i = p.size() - 1; i > 0; i--) {
    float r = random_number();
    int target = int(r * (i+1));
    int tmp = p[i];
    p[i] = p[target];
    p[target] = tmp;
  }
}

Perlin::permutation_t Perlin::generate_permutation() {
  Perlin::permutation_t p;
  for (size_t i = 0; i < p.size(); i++) {
    p[i] = i;
  }
  permute(p);
  return p;
}

float Perlin::noise(const Vec3 &p) const {
  float u = p.x() - floor(p.x());
  float v = p.y() - floor(p.y());
  float w = p.z() - floor(p.z());
  u = u * u * (3 - 2 * u);
  v = v * v * (3 - 2 * v);
  w = w * w * (3 - 2 * w);

  int i = abs(floor(p.x()));
  int j = abs(floor(p.y()));
  int k = abs(floor(p.z()));

  float c[2][2][2];
  for (int di=0; di<2; di++) {
    for (int dj=0; dj<2; dj++) {
      for (int dk=0; dk<2; dk++) {
        int idx = (
          _permutation_x[(i + di) & 255] ^
          _permutation_y[(j + dj) & 255] ^
          _permutation_y[(k + dk) & 255]
        );
        c[di][dj][dk] = _data[idx];
      }
    }
  }
  
  return trilerp(c, u, v, w);
}