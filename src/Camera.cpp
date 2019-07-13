#include "raytracer/Camera.h"

Camera::Camera(vec3 position, vec3 target, vec3 vup, float vfov, float aspect)
: _vertical_fov(vfov)
, _aspect(aspect)
, _origin(position)
{
  float theta = vfov * M_PI / 180.0f;
  float half_height = tan(theta/2.0f);
  float half_width = aspect * half_height;

  vec3 u, v, w;
  w = unit_vector(_origin - target);
  u = unit_vector(cross(vup, w));
  v = cross(w, u);

  _lower_left_corner = _origin - half_width * u - half_height * v - w;
  _horizontal = 2.0f * half_width * u;
  _vertical = 2.0f * half_height * v;
}

ray Camera::get_ray(float u, float v) {
  return ray(_origin, _lower_left_corner + u *_horizontal + v * _vertical);
}