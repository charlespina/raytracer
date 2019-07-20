#ifndef RT_SCENE_H
#define RT_SCENE_H

#include "raytracer/IHitable.h"
#include "raytracer/HitRecord.h"

class Scene : public IHitable {
public:
  virtual bool hit(const ray& r, float tmin, float tmax, HitRecord &record) const override;
  virtual bool bounding_box(float t0, float t1, AxisAlignedBoundingBox& aabb) const override;
  virtual ~Scene() {}
  
public:
  std::vector<std::shared_ptr<IHitable> > _geometries;
};

bool Scene::hit(const ray& r, float t_min, float t_max, HitRecord &record) const {
  bool hit_anything = false;
  float closest_so_far = t_max;
  HitRecord temp_record;
  for (const auto &geom : _geometries) {
    if (geom->hit(r, t_min, closest_so_far, temp_record)) {
      hit_anything = true;
      closest_so_far = temp_record.t;
      record = temp_record;
    }
  }
  return hit_anything;
}

bool Scene::bounding_box(float t0, float t1, AxisAlignedBoundingBox& aabb) const {
  if (_geometries.empty()) return false;

  AxisAlignedBoundingBox temp_aabb;
  if (!_geometries[0]->bounding_box(t0, t1, temp_aabb)) {
    return false;
  }

  aabb = temp_aabb;
  bool all_valid = std::all_of(_geometries.begin(), _geometries.end(), [t0, t1, &temp_aabb, &aabb](const auto &geom) -> bool {
    bool result = geom->bounding_box(t0, t1, temp_aabb);
    if (result) aabb.combine(temp_aabb);
    return result;
  });

  return all_valid;
}

#endif