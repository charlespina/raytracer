#ifndef RT_RECTANGLE_H
#define RT_RECTANGLE_H

#include "raytracer/AxisAlignedBoundingBox.h"
#include "raytracer/materials/materials.h"
#include "raytracer/shapes/Shape.h"
#include "raytracer/shapes/Mesh.h"
#include "raytracer/Ray.h"
#include "raytracer/Vec3.h"

namespace raytracer {

class MeshRectangle : public Mesh {
public:
  MeshRectangle(size_t num_segments, size_t num_sections, std::shared_ptr<Material> material);
  virtual ~MeshRectangle() {}

  float pdf_value(const Vec3 &o, const Vec3 &v) const override {
    SurfaceInteraction record;
    if (this->hit(Ray(o, v, 0), 0.001f, std::numeric_limits<float>::max(), record)) {
      float area = _aabb.min().cross(_aabb.max()).norm();
      float distance_squared = record.t * record.t * v.squaredNorm();
      float cosine = std::abs(v.normalized().dot(record.normal));
      return distance_squared / (cosine * area);
    }
    return 0;
  }

  Vec3 random(const Vec3 &o) const override {
    Vec3 delta = _aabb.max() - _aabb.min();
    Vec3 random_point = Vec3(random_number() * delta.x(), random_number() * delta.y(), random_number() * delta.z()) + _aabb.min();
    return random_point - o;
  }

public:
  AxisAlignedBoundingBox _aabb;
};

} // raytracer

#endif