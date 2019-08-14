#ifndef RT_RECTANGLE_H
#define RT_RECTANGLE_H

#include "raytracer/materials.h"
#include "raytracer/objects/IObject.h"
#include "raytracer/objects/Mesh.h"
#include "raytracer/Vec3.h"

namespace raytracer {

class MeshRectangle : public Mesh {
public:
  MeshRectangle(size_t num_segments, size_t num_sections, std::shared_ptr<Material> material);
  MeshRectangle(Vec3 pt0, Vec3 pt1, Vec3 pt2, Vec3 pt3, std::shared_ptr<Material> material);
  virtual ~MeshRectangle() {}
};

} // raytracer

#endif