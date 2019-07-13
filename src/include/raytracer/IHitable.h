#ifndef RT_IHITABLE_H
#define RT_IHITABLE_H

#include "raytracer/HitRecord.h"
#include "raytracer/ray.h"

class IHitable {
public:
  virtual ~IHitable() {};
  virtual bool hit(const ray& r, float t_min, float t_max, HitRecord &record) const = 0;
};

#endif