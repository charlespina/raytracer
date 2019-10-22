#include "raytracer/shapes/Mesh.h"
#include <cassert>

namespace raytracer {

Mesh::Mesh(const std::vector<Vec3> &vertex_triplets) { 
  assert(vertex_triplets.size() % 3 == 0);
  _vertices = vertex_triplets;
  assert(_vertices.size() == vertex_triplets.size());
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

bool Mesh::hit(const Ray& r, float t_min, float t_max, SurfaceInteraction &record) const {
  float closest_t = std::numeric_limits<float>::max();
  bool did_hit = false;

  for (size_t tri=0; tri<_faces.size(); tri++) {
    const Vec3 & N = _face_normals[tri];

    const Vec3 &v0 = get_face_vertex(tri, 0);
    const Vec3 &v1 = get_face_vertex(tri, 1);
    const Vec3 &v2 = get_face_vertex(tri, 2);

    Vec3 edge1 = v1 - v0;
    Vec3 edge2 = v2 - v0;
    Vec3 h = r.direction().cross(edge2);

    float a = edge1.dot(h);
    if (std::abs(a) < std::numeric_limits<float>::epsilon())
      continue; // ray parallel to tri
    
    float f = 1.0f/a;
    Vec3 s = r.origin() - v0;
    float u = f * s.dot(h);
    if (u < 0.0f || u > 1.0f)
      continue;
    
    Vec3 q = s.cross(edge1);
    float v = f * r.direction().dot(q);
    if (v < 0.0f || (u + v) > 1.0f)
      continue;
    
    float t = f * edge2.dot(q);
    if (t < std::numeric_limits<float>::epsilon())
      continue; // line intersection, but not ray
    
    if (t < t_min || t > t_max)
      continue;
    
    if (t > closest_t)
      continue;

    record.p = r.point_at_parameter(t);
    closest_t = record.t = t;
    record.normal = N;
    record.material = _material.get();
    did_hit = true;
  }

  return did_hit;
}

bool Mesh::bounding_box(float t0, float t1, AxisAlignedBoundingBox& aabb) const {
  AxisAlignedBoundingBox box;
  box.setEmpty();

  for (const auto &v : _vertices) {
    box.extend(v);
  }
  aabb = box;
  return true;
}

} // raytracer