#include "raytracer/Rectangle.h"

namespace raytracer {

Rectangle::Rectangle(Vec3 pt0, Vec3 pt1, std::shared_ptr<Material> mat) : _material(mat) {
  Vec3 origin = (pt0 + pt1)/2.0f;
  Vec3 normal = unit_vector(cross(pt0, pt1));
  // float width = 2.0f * (pt0 - origin);
  // float height = 2.0f * (pt1 - origin);
  _plane = Plane(origin, normal, _material);
}

bool Rectangle::hit(const Ray &r, float t_min, float t_max, HitRecord &record) const {
  return false;
}

bool Rectangle::bounding_box(float t0, float t1, AxisAlignedBoundingBox &aabb) const {
  return false;
}

} // raytracer