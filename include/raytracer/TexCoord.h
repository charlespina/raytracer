#ifndef RT_TEXCOORD_H
#define RT_TEXCOORD_H

#include "raytracer/Vec3.h"

class TexCoord : public Vec3 {
public:
  TexCoord() : Vec3(0, 0, 0) {}
  TexCoord(float u, float v) : Vec3(u, v, 0) {}
  TexCoord(float u, float v, float w) : Vec3(u, v, w) {}
  ~TexCoord() {}

  inline float u() const { return x(); }
  inline float v() const { return y(); }
  inline float w() const { return z(); }
};

#endif