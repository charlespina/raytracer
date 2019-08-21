#include "raytracer/shapes/ConstantMedium.h"
#include "raytracer/materials.h"

using namespace raytracer;

ConstantMedium::ConstantMedium(std::shared_ptr<Shape> object, float density, std::shared_ptr<Texture> albedo)
: _object(object)
, _density(density)
{
  _material = std::make_shared<Isotropic>(albedo);
}

bool ConstantMedium::hit(const Ray& r, float t_min, float t_max, HitRecord &record) const {
  HitRecord hita, hitb;
  if (!_object->hit(r, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), hita))
    return false;
  if (!_object->hit(r, hita.t + 0.0001f, std::numeric_limits<float>::max(), hitb))
    return false;
  
  hita.t = std::max(hita.t, t_min);
  hitb.t = std::min(hitb.t, t_max);
  if (hita.t >= hitb.t) // if we hit the outside of the object after we hit the inside, no good!
    return false;

  if (hita.t < 0) // if we are inside of the object, and the outer wall is behind us
    hita.t = 0;
  
  float distance_inside_boundary = ((hitb.t - hita.t) * r.direction()).norm();
  float hit_distance = -(1.0f/_density)*std::logf(random_number());
  if (hit_distance >= distance_inside_boundary) return false;
  record.t = hita.t + hit_distance / r.direction().norm();
  record.p = r.point_at_parameter(record.t);
  record.normal = Vec3::UnitX(); // arbitrary
  record.material = _material.get();
  return true;
}

bool ConstantMedium::bounding_box(float t0, float t1, AxisAlignedBoundingBox& aabb) const {
  return _object->bounding_box(t0, t1, aabb);
}