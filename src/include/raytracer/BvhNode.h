#ifndef RT_BVHNODE_H
#define RT_BVHNODE_H

#include <vector>
#include "raytracer/IHitable.h"

class BvhNode : public IHitable {
public:
  using iterator_t = std::vector<std::shared_ptr<IHitable> >::iterator;

  BvhNode() {}
  BvhNode(iterator_t list_begin, iterator_t list_end, float t0, float t1);

  virtual bool hit(const ray& r, float t_min, float t_max, HitRecord &record) const;
  virtual bool bounding_box(float t0, float t1, AxisAlignedBoundingBox& aabb) const;

private:
  std::shared_ptr<IHitable> _left;
  std::shared_ptr<IHitable> _right;
  AxisAlignedBoundingBox _aabb;
};

#endif // header guard