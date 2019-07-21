#ifndef RT_PERLIN_H
#define RT_PERLIN_H

#include "raytracer/Vec3.h"
#include <array>

class Perlin {
public:
  Perlin();
  float noise(const Vec3 &p) const;

private:
  using data_t = std::array<float, 256>;
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

#endif