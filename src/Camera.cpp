#include "raytracer/Camera.h"
#include "raytracer/random_numbers.h"

#define PI 3.14159265358979323846f

namespace raytracer {

Camera::Camera(Eigen::Affine3f transform,
  float vfov, float aspect, float aperture, float focus_distance)
: _vertical_fov(vfov)
, _aspect(aspect)
, _lens_radius(aperture/2.0f)
, _transform(transform)
{
  float theta = vfov * PI / 180.0f;
  float half_height = tan(theta/2.0f);
  float half_width = aspect * half_height;

  _origin = _transform * Vec3(0, 0, 0);
  Vec3 forward = _transform.linear() * Vec3::UnitZ();
  _up = _transform.linear() * Vec3::UnitY();
  _right = unit_vector(cross(_up, forward));

  _lower_left_corner = - half_width * focus_distance * _right - half_height * focus_distance * _up - focus_distance * forward;
  _horizontal = 2.0f * half_width * focus_distance * _right;
  _vertical = 2.0f * half_height * focus_distance * _up;
}

Ray Camera::get_ray(float s, float t, float time_begin, float time_end) {
  Vec3 rd = _lens_radius * random_in_unit_disk();
  Vec3 offset = _right * rd.x() + _up * rd.y();
  float time = time_begin + random_number() * (time_end - time_begin);
  return Ray(_origin + offset, (_lower_left_corner + s * _horizontal + t * _vertical - offset).normalized(), time);
}

} // raytracer