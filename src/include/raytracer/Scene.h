#ifndef RT_SCENE_H
#define RT_SCENE_H

#include "raytracer/IHitable.h"
#include "raytracer/HitRecord.h"

class Scene : public IHitable {
public:
  virtual bool hit(const ray& r, float tmin, float tmax, HitRecord &record) const override;
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

#endif