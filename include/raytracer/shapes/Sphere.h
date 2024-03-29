#ifndef RT_SPHERE_H
#define RT_SPHERE_H

#include <memory>

#include "raytracer/materials/materials.h"
#include "raytracer/shapes/Shape.h"
#include "raytracer/Ray.h"
#include "raytracer/Vec3.h"

namespace raytracer {

class Sphere : public Shape {
public:
  Sphere() = delete;
  Sphere(const Vec3 &center, float r, std::shared_ptr<Material> mat, Vec3 velocity=Vec3(0,0,0));
  virtual ~Sphere() {};
  virtual bool hit(const Ray& r, float tmin, float tmax, SurfaceInteraction &record) const override;
  virtual bool bounding_box(float t0, float t1, AxisAlignedBoundingBox& aabb) const override;
  Vec3 center(float t) const;
  
public:
  Vec3 _center;
  float _radius;
  Vec3 _velocity;
  std::shared_ptr<Material> _material;
};

} // raytracer

#endif