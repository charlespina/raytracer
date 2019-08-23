#include "raytracer/materials/pdfs.h"

using namespace raytracer;

CosinePDF::CosinePDF(const Vec3 &w) : _basis(w) {}

float CosinePDF::value(const Vec3 &dir) const {
  float cosine = dir.dot(_basis._w);
  if (cosine > 0)
    return cosine/RT_PI;
  else
    return 0;
}

Vec3 CosinePDF::generate() const {
  return _basis.local(random_cosine_direction());
}
