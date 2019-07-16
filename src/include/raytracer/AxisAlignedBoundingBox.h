#ifndef RT_AXISALIGNEDBOUNDINGBOX_H
#define RT_AXISALIGNEDBOUNDINGBOX_H

#include "raytracer/vec3.h"
#include "raytracer/ray.h"

class AxisAlignedBoundingBox {
public:
  AxisAlignedBoundingBox() {}
  AxisAlignedBoundingBox(const vec3 &imin, const vec3 &imax);
  const vec3 &min() const { return _min; }
  const vec3 &max() const { return _max; }

  bool hit(const ray& r, float tmin, float tmax) const;

  AxisAlignedBoundingBox& combine(const AxisAlignedBoundingBox &aabb);

protected:
  vec3 _min;
  vec3 _max;
};

#endif // header guard