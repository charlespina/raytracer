
#ifndef RT_CAMERA_H
#define RT_CAMERA_H

#include "raytracer/ray.h"
#include "raytracer/vec3.h"

class Camera {
public:
  Camera(vec3 position, vec3 target, vec3 vup, float vertical_fov, float aspect);
  ray get_ray(float u, float v);
  
public:
  float _vertical_fov;
  float _aspect;
  float _aperture;
  vec3 _lower_left_corner;
  vec3 _horizontal;
  vec3 _vertical;
  vec3 _origin;
};

#endif