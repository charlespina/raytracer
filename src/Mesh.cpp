#include "raytracer/Mesh.h"
#include <cassert>

namespace raytracer {

Mesh::Mesh(const std::vector<Vec3> &vertex_triplets) : _vertices(vertex_triplets) {
  assert(vertex_triplets.size() % 3 == 0);
  _triangle_indices.resize(vertex_triplets.size());
  for (size_t vi=0; vi<vertex_triplets.size(); vi++) {
   _triangle_indices[vi] = vi; 
  }
}

bool Mesh::hit(const Ray& r, float t_min, float t_max, HitRecord &record) const {
  // todo!
  return false;
}

bool Mesh::bounding_box(float t0, float t1, AxisAlignedBoundingBox& aabb) const {
  for (const auto &v : _vertices) {
    aabb.extend(v);
  }
  return true;
}

}