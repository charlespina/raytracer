#include "raytracer/Sphere.h"

Sphere::Sphere(const Vec3 &center, float r, std::shared_ptr<Material> mat, Vec3 velocity)
: _center(center)
, _radius(r)
, _velocity(velocity)
, _material(mat)
{
}

bool Sphere::bounding_box(float t0, float t1, AxisAlignedBoundingBox& aabb) const {
  auto b_t0 = AxisAlignedBoundingBox(center(t0) - Vec3(_radius), center(t0) + Vec3(_radius));
  auto b_t1 = AxisAlignedBoundingBox(center(t1) - Vec3(_radius), center(t1) + Vec3(_radius));
  aabb = b_t0.combine(b_t1);
  return true;
}

Vec3 Sphere::center(float t) const {
  return _center + _velocity * t;
}

bool Sphere::hit(const Ray& r, float t_min, float t_max, HitRecord &record) const {
  Vec3 p_center = center(r.time());
  Vec3 oc = r.origin() - p_center;
  float a = dot(r.direction(), r.direction());
  float b = 2.0f * dot(oc, r.direction());
  float c = dot(oc, oc) - _radius * _radius;
  float discriminant = b*b - 4.0f*a*c;

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
    record.normal = (record.p - p_center) / _radius;
    record.material = _material.get();
    return true;
  }
  
  return false;
};