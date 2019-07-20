
#ifndef RT_CAMERA_H
#define RT_CAMERA_H

#include "raytracer/ray.h"
#include "raytracer/vec3.h"

class Camera {
public:
  Camera(vec3 position, vec3 target, vec3 vup, float vertical_fov, float aspect,
    float aperture, float focus_distance);
  ray get_ray(float u, float v, float t_begin=0.0f, float t_end=0.0f);
  
public:
  float _vertical_fov;
  float _aspect;
  float _lens_radius;
  vec3 _lower_left_corner;
  vec3 _horizontal;
  vec3 _vertical;
  vec3 _origin;
  vec3 _forward, _up, _right;
};

#endif