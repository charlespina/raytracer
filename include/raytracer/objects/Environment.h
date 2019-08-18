#ifndef RT_ENVIRONMENT_H_
#define RT_ENVIRONMENT_H_

#include "raytracer/objects/IObject.h"
#include "raytracer/objects/InvertedObject.h"
#include "raytracer/objects/Sphere.h"
#include "raytracer/materials.h"

namespace raytracer {

class Environment : public IObject {
public:
  Environment(std::shared_ptr<Texture> texture) {
    float sufficiently_large_size = 100000.0f;
    auto material = std::make_shared<DiffuseLight>(texture);
    _object = std::make_shared<InvertedObject>(std::make_shared<Sphere>(Vec3(0, 0, 0), 
      sufficiently_large_size,
      material
    ));
  }

  virtual ~Environment() {}

  bool hit(const Ray& r, float t_min, float t_max, HitRecord &record) const override {
    return _object->hit(r, t_min, t_max, record);
  }

  bool bounding_box(float t0, float t1, AxisAlignedBoundingBox& aabb) const override {
    return _object->bounding_box(t0, t1, aabb);
  }

public:
  std::shared_ptr<IObject> _object;
};

} // namespace

#endif // header guard