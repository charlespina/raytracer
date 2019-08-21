#ifndef RT_GROUP_H_
#define RT_GROUP_H_

#include <vector>
#include "raytracer/objects/IObject.h"

namespace raytracer {

class Group : public IObject {
public:
  bool hit(const Ray& r, float tmin, float tmax, HitRecord &record) const override;
  bool bounding_box(float t0, float t1, AxisAlignedBoundingBox& aabb) const override;

  void add_object(std::shared_ptr<IObject> object) {
    _children.push_back(object);
  }

  float pdf_value(const Vec3 &o, const Vec3 &dir) const override;
  Vec3 random(const Vec3 &o) const override;

public:
  std::vector<std::shared_ptr<IObject>> _children;
};

} // raytracer

#endif // header guard