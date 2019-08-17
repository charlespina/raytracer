#include "raytracer/objects/ObjectInstance.h"
#include <vector>

namespace raytracer {

ObjectInstance::ObjectInstance(std::shared_ptr<IObject> hitable, const Eigen::Affine3f &transform, std::shared_ptr<Material> mat)
: _hitable(hitable)
, _material(mat)
{
  _matrices = new mat_container_t();
  set_transform(transform);
}

void ObjectInstance::set_transform(const Eigen::Affine3f &transform) {
  _matrices->_transform = transform;
  _matrices->_inverse_transform = transform.inverse();
  _matrices->_normal_matrix = _matrices->_inverse_transform.linear().transpose();
  _matrices->_inverse_normal_matrix = _matrices->_inverse_transform.linear().inverse().transpose();
}

bool ObjectInstance::hit(const Ray &r, float t_min, float t_max, HitRecord &record) const {
  // transform ray to object space, since hit function is not aware of instance transform
  Vec3 o = _matrices->_inverse_transform * r.origin();
  Vec3 d = (_matrices->_inverse_normal_matrix * r.direction()).normalized();
  Ray ray_object(o, d, r.time());

  bool did_hit = _hitable->hit(ray_object, t_min, t_max, record);

  // convert the hit point and normal to world space
  if (did_hit) {
    record.p = _matrices->_transform * record.p;
    record.normal = _matrices->_normal_matrix * record.normal;
    // original t is along transformed ray - we can effectively get the t of actual ray by getting the distance between the hit and origin
    record.t = (record.p - r.origin()).norm();
    if (_material) record.material = _material.get(); 
  }

  return did_hit;
}

bool ObjectInstance::bounding_box(float t0, float t1, AxisAlignedBoundingBox &aabb) const {
  static std::vector<AxisAlignedBoundingBox::CornerType> corners = {
    AxisAlignedBoundingBox::BottomLeftCeil,
    AxisAlignedBoundingBox::BottomLeftFloor,
    AxisAlignedBoundingBox::BottomRightCeil,
    AxisAlignedBoundingBox::BottomRightFloor,
    AxisAlignedBoundingBox::TopLeftCeil,
    AxisAlignedBoundingBox::TopLeftFloor,
    AxisAlignedBoundingBox::TopRightCeil,
    AxisAlignedBoundingBox::TopRightFloor,
  };


  AxisAlignedBoundingBox untransformed_bbox;
  bool has_bbox = _hitable->bounding_box(t0, t1, untransformed_bbox);
  if (!has_bbox) return false;

  AxisAlignedBoundingBox result;
  result.setEmpty();

  for (auto corner : corners) {
    result.extend(_matrices->_transform * untransformed_bbox.corner(corner));
  }

  aabb = result;
  return true;
}

} // raytracer