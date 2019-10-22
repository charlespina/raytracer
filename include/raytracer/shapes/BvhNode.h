#ifndef RT_BVHNODE_H
#define RT_BVHNODE_H

#include <vector>
#include "raytracer/shapes/Shape.h"

namespace raytracer {

class BvhNode : public Shape {
public:
  using iterator_t = std::vector<std::shared_ptr<Shape> >::iterator;

  BvhNode() {}
  BvhNode(iterator_t list_begin, iterator_t list_end, float t0, float t1);

  virtual bool hit(const Ray& r, float t_min, float t_max, SurfaceInteraction &record) const;
  virtual bool bounding_box(float t0, float t1, AxisAlignedBoundingBox& aabb) const;
  size_t count() const;
  size_t count_left() const;
  size_t count_right() const;

private:
  std::shared_ptr<Shape> _left;
  std::shared_ptr<Shape> _right;
  AxisAlignedBoundingBox _aabb;
};

} // raytracer

#endif // header guard