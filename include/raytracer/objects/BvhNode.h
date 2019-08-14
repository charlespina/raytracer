#ifndef RT_BVHNODE_H
#define RT_BVHNODE_H

#include <vector>
#include "raytracer/objects/IObject.h"

namespace raytracer {

class BvhNode : public IObject {
public:
  using iterator_t = std::vector<std::shared_ptr<IObject> >::iterator;

  BvhNode() {}
  BvhNode(iterator_t list_begin, iterator_t list_end, float t0, float t1);

  virtual bool hit(const Ray& r, float t_min, float t_max, HitRecord &record) const;
  virtual bool bounding_box(float t0, float t1, AxisAlignedBoundingBox& aabb) const;
  size_t count() const;
  size_t count_left() const;
  size_t count_right() const;

private:
  std::shared_ptr<IObject> _left;
  std::shared_ptr<IObject> _right;
  AxisAlignedBoundingBox _aabb;
};

} // raytracer

#endif // header guard