#include "raytracer/Sphere.h"

Sphere::Sphere(const vec3 &center, float r, std::shared_ptr<Material> mat)
: _center(center)
, _radius(r)
, _material(mat)
{
}

bool Sphere::hit(const ray& r, float t_min, float t_max, HitRecord &record) const {
  vec3 oc = r.origin() - _center;
  float a = dot(r.direction(), r.direction());
  float b = 2.0f * dot(oc, r.direction());
  float c = dot(oc, oc) - _radius * _radius;
  float discriminant = b*b - 4.0*a*c;

  if (discriminant < 0) return false;
  
  float maybe_t = (-b - sqrtf(discriminant)) / (2.0f * a);
  float t = -1;
  if (maybe_t < t_max && maybe_t > t_min) {
    t = maybe_t;
  }

  maybe_t = (-b + sqrtf(discriminant)) / (2.0f * a);
  if (t < 0 && maybe_t < t_max && maybe_t > t_min) {
    t = maybe_t;
  }

  if (t > 0) {
    record.t = t;
    record.p = r.point_at_parameter(t);
    record.normal = (record.p - _center) / _radius;
    record.material = _material.get();
    return true;
  }
  
  return false;
};