#ifndef RT_RECTANGLE_H
#define RT_RECTANGLE_H

#include "raytracer/IHitable.h"
#include "raytracer/materials.h"
#include "raytracer/Plane.h"
#include "raytracer/Vec3.h"

namespace raytracer {

class Rectangle : public IHitable {
public:
  Rectangle(Vec3 pt0, Vec3 pt1, std::shared_ptr<Material> material);

  virtual bool hit(const Ray& r, float t_min, float t_max, HitRecord &record) const override;
  virtual bool bounding_box(float t0, float t1, AxisAlignedBoundingBox& aabb) const override;

private:
  Plane _plane;
  float _width;
  float _height;
  std::shared_ptr<Material> _material;
};

} // raytracer

#endif