#include "raytracer/Instance.h"


Instance::Instance(std::shared_ptr<IHitable> hitable)
: _hitable(hitable)
{
  _transform.setIdentity();
}

bool Instance::hit(const Ray &r, float t_min, float t_max, HitRecord &record) const {
  return false; // todo
}

bool Instance::bounding_box(float t0, float t1, AxisAlignedBoundingBox &aabb) const {
  //Eigen::Vector3f 
  //Vec3 
  //AxisAlignedBoundingBox
  return false; // todo
}