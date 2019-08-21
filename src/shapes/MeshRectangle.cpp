#include "raytracer/shapes/MeshRectangle.h"

using namespace raytracer;

namespace {
  std::vector<Vec3> create_rectangle_mesh(size_t num_segments, size_t num_sections) {
    std::vector<Vec3> result;

    auto get_vertex = [num_segments, num_sections](size_t i, size_t j) -> Vec3 {
        float s = i/float(num_segments-1);
        float t = j/float(num_sections-1);
        return Vec3(s - 0.5f, 0, t-0.5f);
    };

    for (size_t i=0; i<num_segments-1; i++) {
      for (size_t j=0; j<num_sections-1; j++) {
        Vec3 v0 = get_vertex(i, j);
        Vec3 v1 = get_vertex(i, j+1);
        Vec3 v2 = get_vertex(i+1, j);
        Vec3 v3 = get_vertex(i+1, j+1);

        result.push_back(v0);
        result.push_back(v1);
        result.push_back(v2);

        result.push_back(v1);
        result.push_back(v3);
        result.push_back(v2);
      }
    }

    return result;
  }
} // internal namespace

MeshRectangle::MeshRectangle(size_t num_segments, size_t num_sections, std::shared_ptr<Material> mat)
: Mesh(create_rectangle_mesh(num_segments, num_sections))
{
  _material = mat;
  _aabb = AxisAlignedBoundingBox(Vec3(-0.5f, 0, -0.5f), Vec3(0.5f, 0, 0.5f));
}

/*
MeshRectangle::MeshRectangle(Vec3 pt0, Vec3 pt1, Vec3 pt2, Vec3 pt3, 
  std::shared_ptr<Material> mat) 
: Mesh({pt0, pt1, pt2, pt0, pt2, pt3})
{
  _material = mat;
}
*/