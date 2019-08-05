#include "raytracer/AxisAlignedBoundingBox.h"
#include <algorithm>

namespace raytracer {

namespace {
  inline float ffmin(float a, float b) {
    return a < b? a : b;
  }

  inline float ffmax(float a, float b) {
    return a > b? a : b;
  }
} // internal

bool aabb_hit(const AxisAlignedBoundingBox &box, const Ray& r, float tmin, float tmax) {
  for (int a=0; a<3; a++) {
    float t0 = std::min(
      (box.min()[a] - r.origin()[a]) / r.direction()[a],
      (box.max()[a] - r.origin()[a]) / r.direction()[a]
    );

    float t1 = std::max(
      (box.min()[a] - r.origin()[a]) / r.direction()[a],
      (box.max()[a] - r.origin()[a]) / r.direction()[a]
    );

    tmin = std::max(t0, tmin);
    tmax = std::min(t1, tmax);
    if (tmax <= tmin)
      return false;
  }

  return true;
  /*
  for (int a=0; a<3; a++) {
    float inv_d = 1.0f / r.direction()[a];
    float t0 = (_min[a] - r.origin()[a]) * inv_d;
    float t1 = (_max[a] - r.origin()[a]) * inv_d;
    if (inv_d < 0.0f)
      std::swap(t0, t1);
    tmin = t0 > tmin ? t0 : tmin;
    tmax = t1 < tmax ? t1 : tmax;
    if (tmax <= tmin)
      return false;
  }
  */

  return true;
}

} // raytracer