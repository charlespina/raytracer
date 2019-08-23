#ifndef RT_MESH_H
#define RT_MESH_H

#include "raytracer/materials/materials.h"
#include "raytracer/shapes/Shape.h"
#include "raytracer/shapes/Plane.h"
#include "raytracer/Vec3.h"

namespace raytracer {

class Mesh : public Shape {
public:
  struct Face {
    Face()
    : v0(0)
    , v1(0)
    , v2(0)
    {}
  
    Face(size_t idx0, size_t idx1, size_t idx2) 
    : v0(idx0)
    , v1(idx1)
    , v2(idx2)
    {}

    size_t operator[](size_t idx) const {
      if(idx == 0) return v0;
      else if(idx == 1) return v1;
      else return v2;
    }

    size_t v0 = 0;
    size_t v1 = 0;
    size_t v2 = 0;
  };

  Mesh() = delete;
  Mesh(const std::vector<Vec3> &vertex_triplets);
  virtual ~Mesh() {}

  virtual bool hit(const Ray& r, float t_min, float t_max, HitRecord &record) const override;
  virtual bool bounding_box(float t0, float t1, AxisAlignedBoundingBox& aabb) const override;

  inline const Vec3 & get_face_vertex(size_t face_index, size_t vertex_offset) const {
    return _vertices[get_face_vertex_index(face_index, vertex_offset)];
  }

  inline size_t get_face_vertex_index(size_t face_index, size_t vertex_offset) const {
    return _faces[face_index][vertex_offset];
  }

public:
  std::vector<Vec3> _vertices;
  std::vector<Face> _faces;
  std::vector<Vec3> _face_normals;
  std::shared_ptr<Material> _material;
};

}

#endif