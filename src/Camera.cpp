#include "raytracer/Camera.h"
#include "raytracer/random_numbers.h"

Camera::Camera(vec3 position, vec3 target, vec3 vup, 
  float vfov, float aspect, float aperture, float focus_distance)
: _vertical_fov(vfov)
, _aspect(aspect)
, _lens_radius(aperture/2.0f)
, _origin(position)
{
  float theta = vfov * M_PI / 180.0f;
  float half_height = tan(theta/2.0f);
  float half_width = aspect * half_height;

  _forward = unit_vector(_origin - target);
  _right = unit_vector(cross(vup, _forward));
  _up = cross(_forward, _right);

  _lower_left_corner = - half_width * focus_distance * _right - half_height * focus_distance * _up - focus_distance * _forward;
  _horizontal = 2.0f * half_width * focus_distance * _right;
  _vertical = 2.0f * half_height * focus_distance * _up;
}

ray Camera::get_ray(float s, float t, float time_begin, float time_end) {
  vec3 rd = _lens_radius * random_in_unit_disk();
  vec3 offset = _right * rd.x() + _up * rd.y();
  float time = time_begin + random_number() * (time_end - time_begin);
  return ray(_origin + offset, _lower_left_corner + s * _horizontal + t * _vertical - offset, time);
}