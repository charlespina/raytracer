#include "raytracer/objects/ConstantMedium.h"
#include "raytracer/materials.h"

using namespace raytracer;

ConstantMedium::ConstantMedium(std::shared_ptr<IObject> object, float density, std::shared_ptr<Texture> albedo)
: _object(object)
, _density(density)
{
  _material = std::make_shared<Isotropic>(albedo);
}

bool ConstantMedium::hit(const Ray& r, float t_min, float t_max, HitRecord &record) const {
  HitRecord hita, hitb;
  if (!boundary->hit(r, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), hita))
    return false;
  if (!boundary->hit(r, hita.t + 0.0001f, std::numeric_limits<float>::max(), hitb))
    return false;
  
  if (hita.t < t_min)
    hita.t = t_min;
  if (hitb.t > t_max)
    hitb.t = t_max;
  if (hita.t >= hitb.t)
    return false;
  if (hita.t < 0)
    hita.t = 0;
  
  float distance_inside_boundary =

}

bool ConstantMedium::bounding_box(float t0, float t1, AxisAlignedBoundingBox& aabb) const {
  return _object->bounding_box(t0, t1, aabb);
}