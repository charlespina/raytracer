#include "raytracer/Mesh.h"
#include <cassert>

Mesh::Mesh(std::vector<Vec3> vertex_triplets) : _vertices(vertex_triplets) {
  assert(vertex_triplets.size() % 3 == 0);
  _triangle_indices.resize(vertex_triplets.size());
  for (size_t vi=0; vi<vertex_triplets.size(); vi++) {
   _triangle_indices[vi] = vi; 
  }
}