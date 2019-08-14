#ifndef RT_CONSTANTMEDIUM_H_
#define RT_CONSTANTMEDIUM_H_

#include "raytracer/objects/IObject.h"

class ConstantMedium : public IObject {
public:
  ConstantMedium(std::shared_ptr<IObject> object, float density, std::shared_ptr<Texture> albedo);
  virtual bool hit(const Ray& r, float t_min, float t_max, HitRecord &record) const override;
  virtual bool bounding_box(float t0, float t1, AxisAlignedBoundingBox& aabb) const override;

  virtual ~ConstantMedium() {}

public:
  std::shared_ptr<IObject> _object;
  std::shared_ptr<Material> _material;
  float _density;
};



#endif