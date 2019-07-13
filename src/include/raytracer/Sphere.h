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
  Sphere(const vec3 &center, float r, std::shared_ptr<Material> mat);
  virtual ~Sphere() {};
  virtual bool hit(const ray& r, float tmin, float tmax, HitRecord &record) const override;
  
public:
  vec3 _center;
  float _radius;
  std::shared_ptr<Material> _material;
};

#endif