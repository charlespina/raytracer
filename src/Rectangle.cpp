#include "raytracer/Rectangle.h"

namespace raytracer {

Rectangle::Rectangle(Vec3 pt0, Vec3 pt1, Vec3 pt2, Vec3 pt3, 
  std::shared_ptr<Material> mat) 
: Mesh({pt0, pt1, pt2, pt0, pt2, pt3})
{
  _material = mat;
}

} // raytracer