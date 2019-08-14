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

}
bool ConstantMedium::bounding_box(float t0, float t1, AxisAlignedBoundingBox& aabb) const {
  return _object->bounding_box(t0, t1, aabb);
}