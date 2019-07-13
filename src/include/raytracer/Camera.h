
#ifndef RT_CAMERA_H
#define RT_CAMERA_H

#include "raytracer/ray.h"
#include "raytracer/vec3.h"

class Camera {
public:
  Camera();
  ray get_ray(float u, float v);
  
public:
  vec3 _lower_left_corner;
  vec3 _horizontal;
  vec3 _vertical;
  vec3 _origin;
};

#endif