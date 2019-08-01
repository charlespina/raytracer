#include "raytracer/Instance.h"
#include <vector>

Instance::Instance(std::shared_ptr<IHitable> hitable, const Eigen::Affine3f &transform)
: _hitable(hitable)
{
  _matrices = new mat_container_t();
  set_transform(transform);
}

void Instance::set_transform(const Eigen::Affine3f &transform) {
  _matrices->_transform = transform;
  _matrices->_inverse_transform = transform.inverse();
  _matrices->_normal_matrix = _matrices->_inverse_transform.linear().transpose();
  _matrices->_inverse_normal_matrix = _matrices->_inverse_transform.linear().inverse().transpose();
}

bool Instance::hit(const Ray &r, float t_min, float t_max, HitRecord &record) const {
  // transform ray to object space, since hit function is not aware of instance transform
  Vec3 o = _matrices->_inverse_transform * r.origin();
  Vec3 d = _matrices->_inverse_normal_matrix * r.direction();
  Ray ray_object(o, d, r.time());

  bool did_hit = _hitable->hit(ray_object, t_min, t_max, record);

  // convert the hit point and normal to world space
  if (did_hit) {
    record.p = _matrices->_transform * record.p;
    record.normal = _matrices->_normal_matrix * record.normal;
  }

  return did_hit;
}

bool Instance::bounding_box(float t0, float t1, AxisAlignedBoundingBox &aabb) const {
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
  for (auto corner : corners) {
    result.extend(_matrices->_transform * untransformed_bbox.corner(corner));
  }

  aabb = result;
  return true;
}