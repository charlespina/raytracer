#ifndef RT_IHITABLE_H
#define RT_IHITABLE_H

#include "raytracer/AxisAlignedBoundingBox.h"
#include "raytracer/HitRecord.h"
#include "raytracer/Ray.h"

class IHitable {
public:
  virtual ~IHitable() {};
  virtual bool hit(const Ray& r, float t_min, float t_max, HitRecord &record) const = 0;
  virtual bool bounding_box(float t0, float t1, AxisAlignedBoundingBox& aabb) const = 0;
};

#endif