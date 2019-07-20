#ifndef RT_HITRECORD_H
#define RT_HITRECORD_H

#include "raytracer/vec3.h"

class Material;
struct HitRecord {
  float t;
  vec3 p;
  vec3 normal;
  Material *material;
};

#endif