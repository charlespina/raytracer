#include "raytracer/AxisAlignedBoundingBox.h"
#include <algorithm>

float ffmin(float a, float b) {
  return a < b? a : b;
}

float ffmax(float a, float b) {
  return a > b? a : b;
}

AxisAlignedBoundingBox::AxisAlignedBoundingBox(const Vec3 &imin, const Vec3 &imax) 
: _min(imin)
, _max(imax)
{
}

AxisAlignedBoundingBox& AxisAlignedBoundingBox::combine(const AxisAlignedBoundingBox &other) {
  _min = Vec3(
    std::min(_min.x(), other._min.x()),
    std::min(_min.y(), other._min.y()),
    std::min(_min.z(), other._min.z())
  );
  _max = Vec3(
    std::max(_max.x(), other._max.x()),
    std::max(_max.y(), other._max.y()),
    std::max(_max.z(), other._max.z())
  );
  return *this;
}

bool AxisAlignedBoundingBox::hit(const Ray& r, float tmin, float tmax) const {
  for (int a=0; a<3; a++) {
    float t0 = std::min(
      (_min[a] - r.origin()[a]) / r.direction()[a],
      (_max[a] - r.origin()[a]) / r.direction()[a]
    );

    float t1 = std::max(
      (_min[a] - r.origin()[a]) / r.direction()[a],
      (_max[a] - r.origin()[a]) / r.direction()[a]
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