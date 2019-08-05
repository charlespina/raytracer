#ifndef RT_PERLIN_H
#define RT_PERLIN_H

#include "raytracer/Vec3.h"
#include <array>

namespace raytracer {

class Perlin {
public:
  Perlin();
  float noise(const Vec3 &p) const;
  float turbulent_noise(const Vec3 &p, int depth=7) const;

private:
  using data_t = std::array<Vec3, 256>;
  using permutation_t = std::array<int, 256>;

  data_t generate();
  void permute(permutation_t &p);
  permutation_t generate_permutation();

private:
  data_t _data;
  permutation_t _permutation_x;
  permutation_t _permutation_y;
  permutation_t _permutation_z;
};

} // raytracer

#endif