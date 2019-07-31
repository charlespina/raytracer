#ifndef RT_INSTANCE_H
#define RT_INSTANCE_H

#include "Eigen/Dense"
#include "raytracer/IHitable.h"
#include <memory>

class Instance : public IHitable {
public:
  Instance(std::shared_ptr<IHitable> hitable);
  virtual ~Instance() {}
  virtual bool hit(const Ray& r, float t_min, float t_max, HitRecord &record) const override;
  virtual bool bounding_box(float t0, float t1, AxisAlignedBoundingBox& aabb) const override;

public:
  std::shared_ptr<IHitable> _hitable;
  Eigen::Affine3f _transform;
};

#endif
