#ifndef RT_PLANE_H
#define RT_PLANE_H

#include "raytracer/IHitable.h"
#include "raytracer/vec3.h"

class Plane : public IHitable {
public:
  Plane(const vec3 &position, const vec3 &normal, std::shared_ptr<Material> material);
  virtual ~Plane() {};
  virtual bool hit(const ray& r, float t_min, float t_max, HitRecord &record) const override;
  virtual bool bounding_box(float t0, float t1, AxisAlignedBoundingBox& aabb) const override { return false; }

public:
  vec3 _position;
  vec3 _normal;
  std::shared_ptr<Material> _material;
};

#endif

