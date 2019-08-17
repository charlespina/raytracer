#include "raytracer/Ray.h"

namespace raytracer {

size_t Ray::_ray_count = 0;

Ray::Ray() {
  _ray_count++;
}

Ray::Ray(const Vec3 &origin, const Vec3 &dir, float t) : _origin(origin), _dir(dir.normalized()), _time(t) {
  _ray_count++;
}

Vec3 Ray::point_at_parameter(float t) const { 
  return _origin + t * _dir; 
}

} // raytracer