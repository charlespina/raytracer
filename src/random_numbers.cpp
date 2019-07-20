#include "raytracer/random_numbers.h"

#include <random>

namespace {
  struct RandomNumberGenerator {
    RandomNumberGenerator()
    : generator(std::random_device()())
    , distribution(0, 1)
    {
    }

    float get_random_number() {
        return distribution(generator);
    }

  private:
    std::mt19937 generator;
    std::uniform_real_distribution<float> distribution;
  };
} // internal namespace

float random_number() {
  static RandomNumberGenerator rng;
  return rng.get_random_number();
}

Vec3 random_in_unit_sphere() {
  Vec3 p;
  do {
    p = 2.0f * Vec3(random_number(), random_number(), random_number()) - Vec3(1, 1, 1);
  } while (p.squared_length() >= 1.0f);
  return p;
}

Vec3 random_in_unit_disk() {
  Vec3 p;
  do {
    p = 2.0f * Vec3(random_number(), random_number(), 0.0f) - Vec3(1.0f, 1.0f, 0.0f);
  } while (dot(p, p) >= 1.0f);
  return p;
}