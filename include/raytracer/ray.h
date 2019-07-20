#ifndef RT_RAY_H
#define RT_RAY_H

#include "raytracer/vec3.h"

struct ray {
  ray() {}
  ray(const vec3 &origin, const vec3 &dir, float t) : _origin(origin), _dir(dir), _time(t) {}
  const vec3 &origin() const { return _origin; }
  const vec3 &direction() const { return _dir; }
  float time() const { return _time; }
  vec3 point_at_parameter(float t) const { return _origin + t * _dir; }

  vec3 _origin;
  vec3 _dir;
  float _time;
};

#endif // header guard