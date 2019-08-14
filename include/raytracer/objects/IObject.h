#ifndef RT_IOBJECT_H
#define RT_IOBJECT_H

#include "raytracer/AxisAlignedBoundingBox.h"
#include "raytracer/HitRecord.h"
#include "raytracer/Ray.h"

namespace raytracer {

class IObject {
public:
  virtual ~IObject() {};
  virtual bool hit(const Ray& r, float t_min, float t_max, HitRecord &record) const = 0;
  virtual bool bounding_box(float t0, float t1, AxisAlignedBoundingBox& aabb) const = 0;
};

} // raytracer

#endif