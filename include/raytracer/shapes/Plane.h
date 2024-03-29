#ifndef RT_PLANE_H
#define RT_PLANE_H

#include "raytracer/shapes/Shape.h"
#include "raytracer/Vec3.h"

namespace raytracer {

class Plane : public Shape {
public:
  Plane() {}
  Plane(const Vec3 &position, const Vec3 &normal, std::shared_ptr<Material> material);
  virtual ~Plane() {};
  virtual bool hit(const Ray& r, float t_min, float t_max, SurfaceInteraction &record) const override;
  virtual bool bounding_box(float t0, float t1, AxisAlignedBoundingBox& aabb) const override;

public:
  Vec3 _position;
  Vec3 _normal;
  std::shared_ptr<Material> _material;
};

} // raytracer

#endif