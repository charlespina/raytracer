#ifndef RT_SPHERE_H
#define RT_SPHERE_H

#include <memory>

#include "raytracer/IHitable.h"
#include "raytracer/HitRecord.h"
#include "raytracer/materials.h"
#include "raytracer/ray.h"
#include "raytracer/vec3.h"


class Sphere : public IHitable {
public:
  Sphere() = delete;
  Sphere(const vec3 &center, float r, std::shared_ptr<Material> mat, vec3 velocity=vec3(0,0,0));
  virtual ~Sphere() {};
  virtual bool hit(const ray& r, float tmin, float tmax, HitRecord &record) const override;
  virtual bool bounding_box(float t0, float t1, AxisAlignedBoundingBox& aabb) const override;
  vec3 center(float t) const;
  
public:
  vec3 _center;
  float _radius;
  vec3 _velocity;
  std::shared_ptr<Material> _material;
};

#endif