#ifndef RT_ORTHONORMALBASIS_H
#define RT_ORTHONORMALBASIS_H

#include "raytracer/Vec3.h"

namespace raytracer {

class OrthoNormalBasis {
public:
  OrthoNormalBasis(const Vec3 &w) {
    _w = w.normalized();
    Vec3 a;
    if (std::abs(_w.x()) > 0.9f)
      a = Vec3::UnitY();
    else
      a = Vec3::UnitX();
    
    _v = _w.cross(a).normalized();
    _u = _w.cross(_v);
  }

  Vec3 local(float a, float b, float c) const { return a*_u + b*_v + c *_w; }
  Vec3 local(const Vec3 a) const { return a.x() * _u + a.y() *_v + a.z() *_w; }

public:
  Vec3 _u;
  Vec3 _v;
  Vec3 _w;
};

} // raytracer

#endif // header guard