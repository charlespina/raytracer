#include "raytracer/objects/Sphere.h"
#include "raytracer/math.h"

namespace raytracer {

namespace {
  TexCoord get_sphere_texcoord(const Vec3 &p) {
    float phi = atan2(p.z(), p.x());
    float theta = asin(p.y());

    float u = 1.0f - (phi + RT_PI) / (2 * RT_PI);
    float v = (theta + RT_PI/2.0f) / RT_PI;
  
    TexCoord texcoord(
      u, v
    );

    return texcoord;
  }
}

Sphere::Sphere(const Vec3 &center, float r, std::shared_ptr<Material> mat, Vec3 velocity)
: _center(center)
, _radius(r)
, _velocity(velocity)
, _material(mat)
{
}

bool Sphere::bounding_box(float t0, float t1, AxisAlignedBoundingBox& aabb) const {
  auto b_t0 = AxisAlignedBoundingBox(center(t0) - _radius * Vec3::Ones(), center(t0) + _radius * Vec3::Ones());
  auto b_t1 = AxisAlignedBoundingBox(center(t1) - _radius * Vec3::Ones(), center(t1) + _radius * Vec3::Ones());
  aabb = b_t0.merged(b_t1);
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
    Vec3 P = r.point_at_parameter(t);
    Vec3 N = unit_vector((P - p_center) / _radius);
  
    record.t = t;
    record.p = P;
    record.normal = N;
    record.material = _material.get();
    record.texcoord = get_sphere_texcoord(N);
    return true;
  }
  
  return false;
};

} // raytracer