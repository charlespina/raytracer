#ifndef RT_TEXTURE_H
#define RT_TEXTURE_H

#include "raytracer/Vec3.h"
#include <cmath>

class Texture {
public:
  virtual ~Texture() {};
  virtual Vec3 sample_color(float u, float v, const Vec3 &p) const = 0;
  float sample_scalar(float u, float v, const Vec3 &p) const {
    return sample_color(u, v, p).r();
  }
};

class ConstantTexture : public Texture {
public:
  ConstantTexture() {};
  virtual ~ConstantTexture() {}

  ConstantTexture(const Vec3 & c) : _color(c) {}
  Vec3 sample_color(float u, float v, const Vec3 &p) const override {
    return _color;
  }

  Vec3 _color;
};

class CheckerTexture : public Texture {
public:
  CheckerTexture() {};
  virtual ~CheckerTexture() {}

  CheckerTexture(std::shared_ptr<Texture> even, std::shared_ptr<Texture> odd)
    : _even(even), _odd(odd) {}

  std::shared_ptr<Texture> _even;
  std::shared_ptr<Texture> _odd;

  Vec3 sample_color(float u, float v, const Vec3 &p) const override {
    float rate = 10.0f;
    float sines = sin(rate * p.x()) * sin(rate * p.y()) * sin(rate * p.z());
    if (sines < 0) {
      return _odd->sample_color(u, v, p);
    } else {
      return _even->sample_color(u, v, p);
    }
  }
};

#endif