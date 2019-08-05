#ifndef RT_MESH_H
#define RT_MESH_H

#include "raytracer/IHitable.h"
#include "raytracer/materials.h"
#include "raytracer/Plane.h"
#include "raytracer/Vec3.h"

class Mesh : public IHitable {
public:
  Mesh() = delete;
  Mesh(std::vector<Vec3> vertex_triplets);
  virtual ~Mesh() {}

public:
  std::vector<Vec3> _vertices;
  std::vector<size_t> _triangle_indices;
};

#endif