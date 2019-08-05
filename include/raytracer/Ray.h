#ifndef RT_RAY_H
#define RT_RAY_H

#include "raytracer/Vec3.h"

namespace raytracer {

struct Ray {
  Ray();
  Ray(const Vec3 &origin, const Vec3 &dir, float t);

  const Vec3 &origin() const { return _origin; }
  const Vec3 &direction() const { return _dir; }
  float time() const { return _time; }
  Vec3 point_at_parameter(float t) const;

  Vec3 _origin;
  Vec3 _dir;
  float _time;
  static size_t _ray_count;
};

} // raytracer

#endif // header guard