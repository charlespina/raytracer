#include "raytracer/AxisAlignedBoundingBox.h"

AxisAlignedBoundingBox::AxisAlignedBoundingBox(const vec3 &imin, const vec3 &imax) 
: _min(imin)
, _max(imax)
{
}

AxisAlignedBoundingBox& AxisAlignedBoundingBox::combine(const AxisAlignedBoundingBox &other) {
  _min = vec3(
    std::min(_min.x(), other._min.x()),
    std::min(_min.y(), other._min.y()),
    std::min(_min.z(), other._min.z())
  );
  _max = vec3(
    std::max(_max.x(), other._max.x()),
    std::max(_max.y(), other._max.y()),
    std::max(_max.z(), other._max.z())
  );
  return *this;
}

bool AxisAlignedBoundingBox::hit(const ray& r, float tmin, float tmax) const {
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

  return true;
}