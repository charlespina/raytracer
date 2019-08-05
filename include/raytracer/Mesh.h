#ifndef RT_MESH_H
#define RT_MESH_H

#include "raytracer/IHitable.h"
#include "raytracer/materials.h"
#include "raytracer/Plane.h"
#include "raytracer/Vec3.h"

namespace raytracer {

class Mesh : public IHitable {
public:
  Mesh() = delete;
  Mesh(const std::vector<Vec3> &vertex_triplets);
  virtual ~Mesh() {}

  virtual bool hit(const Ray& r, float t_min, float t_max, HitRecord &record) const override;
  virtual bool bounding_box(float t0, float t1, AxisAlignedBoundingBox& aabb) const override;

public:
  std::vector<Vec3> _vertices;
  std::vector<size_t> _triangle_indices;
};

}

#endif