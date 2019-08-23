#ifndef RT_INVERTEDOBJECT_H_
#define RT_INVERTEDOBJECT_H_

#include "raytracer/shapes/Shape.h"

namespace raytracer {

class InvertedObject : public Shape {
public:
  InvertedObject(std::shared_ptr<Shape> object) : _object(object) {}

  bool hit(const Ray& r, float t_min, float t_max, SurfaceInteraction &record) const override {
    bool result = _object->hit(r, t_min, t_max, record);
    if (result) {
      record.normal = -record.normal;
    }
    return result;
  }

  bool bounding_box(float t0, float t1, AxisAlignedBoundingBox& aabb) const override {
    return _object->bounding_box(t0, t1, aabb);
  }

public:
  std::shared_ptr<Shape> _object;
};

} // namespace

#endif // header guard