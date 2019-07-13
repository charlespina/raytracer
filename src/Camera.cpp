#include "raytracer/Camera.h"

Camera::Camera()
: _lower_left_corner(-2, -1, -1)
, _horizontal(4.0, 0.0, 0.0)
, _vertical(0.0, 2.0, 0.0)
, _origin(0, 0, 0)
{
}

ray Camera::get_ray(float u, float v) {
  return ray(_origin, _lower_left_corner + u *_horizontal + v * _vertical);
}