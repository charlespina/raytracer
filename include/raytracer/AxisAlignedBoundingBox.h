#ifndef RT_AXISALIGNEDBOUNDINGBOX_H
#define RT_AXISALIGNEDBOUNDINGBOX_H

#include "raytracer/Vec3.h"
#include "raytracer/Ray.h"

class AxisAlignedBoundingBox {
public:
  AxisAlignedBoundingBox() {}
  AxisAlignedBoundingBox(const Vec3 &imin, const Vec3 &imax);
  const Vec3 &min() const { return _min; }
  const Vec3 &max() const { return _max; }

  bool hit(const Ray& r, float tmin, float tmax) const;

  AxisAlignedBoundingBox& combine(const AxisAlignedBoundingBox &aabb);

protected:
  Vec3 _min;
  Vec3 _max;
};

#endif // header guard