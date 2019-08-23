#pragma once

#include "raytracer/integrators/Integrator.h"
#include "raytracer/Image.h"
#include "raytracer/Ray.h"
#include "raytracer/Scene.h"

namespace raytracer {

class BasicIntegrator : public Integrator {
public:
  BasicIntegrator() = delete;
  BasicIntegrator(std::shared_ptr<Image<float>> img, 
    size_t samples_per_pixel, 
    size_t max_ray_depth,
    float t_begin, float t_end);

  virtual void render(const Scene &scene) override;

public:
  std::shared_ptr<Image<float>> _image;

private:
  Vec3 Li(const Ray &r, Shape &world, size_t depth);

private:
  Vec3 _miss_color;
  size_t _samples_per_pixel;
  size_t _max_ray_depth;
  float _t_begin;
  float _t_end;
};

} // namespace