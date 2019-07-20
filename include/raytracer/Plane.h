#ifndef RT_PLANE_H
#define RT_PLANE_H

#include "raytracer/IHitable.h"
#include "raytracer/Vec3.h"

class Plane : public IHitable {
public:
  Plane(const Vec3 &position, const Vec3 &normal, std::shared_ptr<Material> material);
  virtual ~Plane() {};
  virtual bool hit(const Ray& r, float t_min, float t_max, HitRecord &record) const override;
  virtual bool bounding_box(float t0, float t1, AxisAlignedBoundingBox& aabb) const override;

public:
  Vec3 _position;
  Vec3 _normal;
  std::shared_ptr<Material> _material;
};

#endif

