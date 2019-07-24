#ifndef RT_TEXTURE_H
#define RT_TEXTURE_H

#include "raytracer/Image.h"
#include "raytracer/Perlin.h"
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

class ImageTexture : public Texture {
public:
  ImageTexture() = delete;
  ImageTexture(const Image<float> &img);
  ImageTexture(const Image<uint8_t> &img_8bpc);

  virtual ~ImageTexture() {}
  virtual Vec3 sample_color(float u, float v, const Vec3 &p) const override;

public:
  Image<float> _image;
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
    float sines = std::sin(rate * p.x()) * std::sin(rate * p.z()); // * std::sin(rate * p.z());
    if (sines < 0) {
      return _odd->sample_color(u, v, p);
    } else {
      return _even->sample_color(u, v, p);
    }
  }
};

class NoiseTexture : public Texture {
public:
  NoiseTexture() = default;
  virtual Vec3 sample_color(float u, float v, const Vec3 &p) const override {
    // return Vec3(1.0f) * _perlin.noise(p);
    // return Vec3(1.0f) * _perlin.turbulent_noise(p);
    return Vec3(1) * 0.5f * (1.0f + sin(_scale * p.z() + 10.0f * _perlin.turbulent_noise(p)));
  }

private:
  Perlin _perlin;
  float _scale = 1.0f;
};

#endif