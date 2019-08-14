
#ifndef RT_CAMERA_H
#define RT_CAMERA_H

#include "raytracer/Ray.h"
#include "raytracer/Vec3.h"

namespace raytracer {

class Camera {
public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
  Camera(Eigen::Affine3f transform,  float vertical_fov, float aspect,
    float aperture, float focus_distance);
  Ray get_ray(float u, float v, float t_begin=0.0f, float t_end=0.0f);
  
public:
  float _vertical_fov;
  float _aspect;
  float _lens_radius;
  Vec3 _lower_left_corner;
  Vec3 _horizontal;
  Vec3 _vertical;
  Vec3 _origin;
  Vec3 _right;
  Vec3 _up;
  Eigen::Affine3f _transform;
};

} // raytracer

#endif