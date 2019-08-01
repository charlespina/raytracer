#ifndef RT_INSTANCE_H
#define RT_INSTANCE_H

#include "Eigen/Dense"
#include "raytracer/IHitable.h"
#include <memory>


class Instance : public IHitable {
public:

  Instance(std::shared_ptr<IHitable> hitable, const Eigen::Affine3f &transform);
  virtual ~Instance() {}
  virtual bool hit(const Ray& r, float t_min, float t_max, HitRecord &record) const override;
  virtual bool bounding_box(float t0, float t1, AxisAlignedBoundingBox& aabb) const override;

  void set_transform(const Eigen::Affine3f &transform);
  const Eigen::Affine3f &transform() const { return _matrices->_transform; }

private:
  struct mat_container_t {
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    Eigen::Affine3f _transform;
    Eigen::Affine3f _normal_matrix;
    Eigen::Affine3f _inverse_transform;
    Eigen::Affine3f _inverse_normal_matrix;
  };

  mat_container_t *_matrices;
  std::shared_ptr<IHitable> _hitable;
};

#endif
