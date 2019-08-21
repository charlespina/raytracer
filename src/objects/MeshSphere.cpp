#include "raytracer/objects/MeshSphere.h"
#include "raytracer/math.h"

using namespace raytracer;

namespace {
  std::vector<Vec3> create_sphere_mesh(float radius, size_t num_segments, size_t num_sections) {

    auto create_vertex = [radius, num_sections, num_segments](size_t i, size_t j) -> Vec3 {
      float s = i % num_segments / (float)(num_segments - 1);
      float t = j % num_sections / (float)(num_sections - 1);

      float theta = t * 2.0f * RT_PI;
      float phi = s * RT_PI;

      Vec3 result;
      result.x() = cos(theta) * sin(phi);
      result.y() = sin(theta) * sin(phi);
      result.z() = cos(phi);

      result *= radius;

      return result;
    };

    std::vector<Vec3> vertices;
    for (size_t x = 0; x < num_segments; x++) {
      for (size_t y = 0; y < num_sections; y++) {
        Vec3 v0 = create_vertex(x,     y);
        Vec3 v1 = create_vertex(x + 1, y);
        Vec3 v2 = create_vertex(x,     y + 1);
        Vec3 v3 = create_vertex(x + 1, y + 1);

        vertices.push_back(v0);
        vertices.push_back(v1);
        vertices.push_back(v2);
        vertices.push_back(v1);
        vertices.push_back(v3);
        vertices.push_back(v2);
      }
    }

    return vertices;
  }

} // internal namespace

MeshSphere::MeshSphere(float radius, size_t segments, size_t sections, std::shared_ptr<Material> mat) 
: Mesh(create_sphere_mesh(radius, segments, sections))
{
  _material = mat;
}
