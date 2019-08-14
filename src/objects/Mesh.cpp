#include "raytracer/objects/Mesh.h"
#include <cassert>

namespace raytracer {

Mesh::Mesh(const std::vector<Vec3> &vertex_triplets) : _vertices(vertex_triplets) {
  assert(vertex_triplets.size() % 3 == 0);
  _faces.resize(vertex_triplets.size() / size_t(3));
  _face_normals.resize(_faces.size());
  for (size_t tri=0; tri<_faces.size(); tri++) {
    _faces[tri].v0 = tri * 3 + 0;
    _faces[tri].v1 = tri * 3 + 1;
    _faces[tri].v2 = tri * 3 + 2;

    const Vec3 &v0 = get_face_vertex(tri, 0);
    const Vec3 &v1 = get_face_vertex(tri, 1);
    const Vec3 &v2 = get_face_vertex(tri, 2);

    Vec3 a = v1 - v0;
    Vec3 b = v2 - v0;
    _face_normals[tri] = a.cross(b).normalized();
  }
}

bool Mesh::hit(const Ray& r, float t_min, float t_max, HitRecord &record) const {
  for (size_t tri=0; tri<_faces.size(); tri++) {
    const Vec3 & N = _face_normals[tri];

    const Vec3 &v0 = get_face_vertex(tri, 0);
    const Vec3 &v1 = get_face_vertex(tri, 1);
    const Vec3 &v2 = get_face_vertex(tri, 2);
    float D = N.dot(v0);
    float denom = N.dot(r.direction());

    if (std::abs(denom) < std::numeric_limits<float>::epsilon())
      continue; // ray is parallel to triangle
    
    float t = (N.dot(r.origin()) + D)
      / denom;

    if (t < t_min || t > t_max) 
      continue; // outside of area of interest

    if (t < 0)
      continue; // intersection is along ray in the wrong direction
    
    Vec3 P = r.point_at_parameter(t);
    
    Vec3 edge0 = v1 - v0;
    Vec3 edge1 = v2 - v1;
    Vec3 edge2 = v0 - v2;
    Vec3 C0 = P - v0;
    Vec3 C1 = P - v1;
    Vec3 C2 = P - v2;

    if ( N.dot(edge0.cross(C0)) > 0
      && N.dot(edge1.cross(C1)) > 0
      && N.dot(edge2.cross(C2)) > 0
    ) {
      // a hit!
      record.p = P;
      record.t = t;
      record.normal = N;
      record.material = _material.get();
      return true;
    }
  }
  return false;
}

bool Mesh::bounding_box(float t0, float t1, AxisAlignedBoundingBox& aabb) const {
  AxisAlignedBoundingBox box;
  for (const auto &v : _vertices) {
    box.extend(v);
  }
  aabb = box;
  return true;
}

} // raytracer