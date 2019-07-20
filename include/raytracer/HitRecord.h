#ifndef RT_HITRECORD_H
#define RT_HITRECORD_H

#include "raytracer/Vec3.h"

class Material;
struct HitRecord {
  float t;
  Vec3 p;
  Vec3 normal;
  Material *material;
};

#endif