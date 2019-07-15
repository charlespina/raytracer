#include "raytracer/Plane.h"
#include "raytracer/ray.h"

Plane::Plane(const vec3 &position, const vec3 &normal, std::shared_ptr<Material> material)
: _position(position)
, _normal(normal)
, _material(material)
{}

bool Plane::hit(const ray& r, float t_min, float t_max, HitRecord &record) const {
  float rdn = dot(_normal, r.direction());

  if (abs(rdn) < std::numeric_limits<float>::epsilon()) return false;

  float t = dot(_position - r.origin(), _normal) / rdn;
  if (t >= t_min && t < t_max) {
    record.t = t;
    record.normal = _normal;
    record.p = t * r.direction() + r.origin();
    record.material = _material.get();
    return true;
  }

  return false;
}