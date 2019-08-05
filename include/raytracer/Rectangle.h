#ifndef RT_RECTANGLE_H
#define RT_RECTANGLE_H

#include "raytracer/IHitable.h"
#include "raytracer/materials.h"
#include "raytracer/Mesh.h"
#include "raytracer/Vec3.h"

namespace raytracer {

class Rectangle : public Mesh {
public:
  Rectangle(Vec3 pt0, Vec3 pt1, Vec3 pt2, Vec3 pt3, std::shared_ptr<Material> material);
  virtual ~Rectangle() {}
};

} // raytracer

#endif