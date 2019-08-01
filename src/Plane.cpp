#include "raytracer/Plane.h"
#include "raytracer/Ray.h"

Plane::Plane(const Vec3 &position, const Vec3 &normal, std::shared_ptr<Material> material)
: _position(position)
, _normal(normal)
, _material(material)
{}

bool Plane::hit(const Ray& r, float t_min, float t_max, HitRecord &record) const {
  float rdn = dot(_normal, r.direction());

  if (abs(rdn) < std::numeric_limits<float>::epsilon()) return false;

  float t = dot(_position - r.origin(), _normal) / rdn;
  if (t >= t_min && t < t_max) {
    record.t = t;
    record.normal = _normal;
    record.p = r.point_at_parameter(t); //t * r.direction() + r.origin();
    record.material = _material.get();
    return true;
  }

  return false;
}
 
bool Plane::bounding_box(float t0, float t1, AxisAlignedBoundingBox& aabb) const {
  AxisAlignedBoundingBox box(
    Vec3{-std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity()}, 
    Vec3{ std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity() }
  );
  aabb = box;
  return true;
}