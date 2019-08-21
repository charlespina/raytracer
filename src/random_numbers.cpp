#include "raytracer/random_numbers.h"
#include "raytracer/math.h"

#include <random>

namespace raytracer {

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
  } while (p.dot(p) >= 1.0f);
  return p;
}

Vec3 random_on_unit_sphere() {
  return random_in_unit_sphere().normalized();
}

Vec3 random_in_unit_disk() {
  Vec3 p;
  do {
    p = 2.0f * Vec3(random_number(), random_number(), 0.0f) - Vec3(1.0f, 1.0f, 0.0f);
  } while (dot(p, p) >= 1.0f);
  return p;
}

Vec3 random_cosine_direction() {
  float r1 = random_number();
  float r2 = random_number();
  float z = sqrtf(1.0f - r2);
  float phi = 2 * RT_PI * r1;
  float x = cos(phi) * 2.0f * sqrtf(r2);
  float y = sin(phi) * 2.0f * sqrtf(r2);
  return Vec3(x, y, z);
}

} // raytracer