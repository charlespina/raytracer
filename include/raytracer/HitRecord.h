#ifndef RT_HITRECORD_H
#define RT_HITRECORD_H

#include "raytracer/TexCoord.h"
#include "raytracer/Vec3.h"

namespace raytracer {

class Material;
struct HitRecord {
  float t;
  Vec3 p;
  Vec3 normal;
  TexCoord texcoord;
  Material *material;
};

} // raytracer

#endif