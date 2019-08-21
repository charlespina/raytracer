#ifndef RT_IOBJECT_H
#define RT_IOBJECT_H

#include "raytracer/AxisAlignedBoundingBox.h"
#include "raytracer/HitRecord.h"
#include "raytracer/Ray.h"

namespace raytracer {

class Shape {
public:
  virtual ~Shape() {};
  virtual bool hit(const Ray& r, float t_min, float t_max, HitRecord &record) const = 0;
  virtual bool bounding_box(float t0, float t1, AxisAlignedBoundingBox& aabb) const = 0;
  virtual float pdf_value(const Vec3 &o, const Vec3 &dir) const { return 0.0f; }
  virtual Vec3 random(const Vec3 &origin) const { return Vec3::UnitX(); }
};

} // raytracer

#endif