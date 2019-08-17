#include "raytracer/objects/IObject.h"

namespace raytracer {

class RectXZ : public IObject {
public:
  RectXZ(float width, float height, std::shared_ptr<Material> mat);
  virtual bool hit(const Ray& r, float t_min, float t_max, HitRecord &record) const override;
  virtual bool bounding_box(float t0, float t1, AxisAlignedBoundingBox& aabb) const override;
  virtual ~RectXZ() {}

public:
  float _width, _height;
  float _x0, _x1, _z0, _z1;
  std::shared_ptr<Material> _material;
};

}