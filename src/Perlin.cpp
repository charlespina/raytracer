#include "raytracer/Perlin.h"
#include "raytracer/random_numbers.h"
#include <cmath>

namespace raytracer {

namespace {

float trilerp(Vec3 c[2][2][2], float u, float v, float w) {
  float uu = u*u*(3-2*u);
  float vv = v*v*(3-2*v);
  float ww = w*w*(3-2*w);

  float accum = 0;
  for (int i=0; i<2; i++) {
    for (int j=0; j<2; j++) {
      for (int k=0; k<2; k++) {
        Vec3 weight_v(u-i, v-j, w-k);
        float r =
          (i * uu + (1 - i) * (1 - uu));
        float s =
          (j * vv + (1 - j) * (1 - vv));
        float t =
          (k * ww + (1 - k) * (1 - ww));
        accum += r * s * t *
          dot(c[i][j][k], weight_v);
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
    data[i] = unit_vector(Vec3(
      2 * random_number() - 1,
      2 * random_number() - 1,
      2 * random_number() - 1
    ));
  }
  return data;
}

void Perlin::permute(permutation_t &p) {
  for (int i = (int)p.size() - 1; i > 0; i--) {
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
    p[i] = (int)i;
  }
  permute(p);
  return p;
}

float Perlin::turbulent_noise(const Vec3 &p, int depth) const {
  float accum = 0;
  Vec3 temp_p = p;
  float weight = 1.0f;
  for (int i=0; i < depth; i++) {
    accum += weight * noise(temp_p);
    weight *= 0.5f;
    temp_p *= 2.0f;
  }

  return std::abs(accum);
}

float Perlin::noise(const Vec3 &p) const {
  int i = int(p.x());
  int j = int(p.y());
  int k = int(p.z());

  float u = p.x() - floor(p.x());
  float v = p.y() - floor(p.y());
  float w = p.z() - floor(p.z());

  Vec3 c[2][2][2];
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

} // raytracer