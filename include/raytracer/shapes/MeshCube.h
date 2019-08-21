#ifndef RT_MESHCUBE_H_
#define RT_MESHCUBE_H_

#include "raytracer/shapes/Shape.h"
#include "raytracer/shapes/Mesh.h"

namespace raytracer {

class MeshCube : public Mesh {
public:
  MeshCube(std::shared_ptr<Material> mat);
};

std::vector<Vec3> create_cube_vertices() {
  std::vector<Vec3> vertices;

  float h = 0.5f;
  std::vector<Vec3> one_face = {
    Vec3(-h,  0, -h),
    Vec3(-h,  0,  h),
    Vec3( h,  0, -h),

    Vec3(-h,  0,  h),
    Vec3( h,  0,  h),
    Vec3( h,  0, -h),
  };

  auto make_faces = [&one_face, &vertices, h](float t, Vec3 axis) -> void {
    Vec3 offset = Vec3(0, h, 0);
    Eigen::AngleAxisf rotation = 
      Eigen::AngleAxisf(t * 2.0f * 3.14159f, axis);

    Eigen::Affine3f tx = 
      // Eigen::Translation3f(-offset) *
      rotation *
      Eigen::Translation3f(offset) *
      Eigen::Affine3f::Identity();

    for (const auto &vert : one_face) {
      Vec3 transformed_vert = tx * vert;
      vertices.push_back(transformed_vert);
    }
  };

  make_faces(0, Vec3::UnitZ());
  make_faces(0.25, Vec3::UnitZ());
  make_faces(0.5,  Vec3::UnitZ());
  make_faces(0.75, Vec3::UnitZ());
  make_faces(0.25, Vec3::UnitX());
  make_faces(0.75, Vec3::UnitX());

  return vertices;
}

MeshCube::MeshCube(std::shared_ptr<Material> mat)
: Mesh(create_cube_vertices()) {
  _material = mat;
}

} // namespace

#endif // header guard