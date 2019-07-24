#ifndef RT_MATERIALS_H
#define RT_MATERIALS_H

#include "raytracer/HitRecord.h"
#include "raytracer/random_numbers.h"
#include "raytracer/Ray.h"
#include "raytracer/textures.h"
#include <memory>

class Material {
public:
  virtual bool scatter(const Ray &iray, const HitRecord &hit, Vec3 &atten, Ray &scattered) const = 0;
};

class Lambertian : public Material {
public:
  Lambertian(const Vec3 &albedo) : _albedo(std::make_shared<ConstantTexture>(albedo)) {}
  Lambertian(std::shared_ptr<Texture> albedo_texture) : _albedo(albedo_texture) {}
  virtual bool scatter(const Ray &iray, const HitRecord &hit, Vec3 &attenuation, Ray &scattered) const override;

public:
  std::shared_ptr<Texture> _albedo;
};

class Dielectric : public Material {
public:
  Dielectric(float roughness, float ior) : _roughness(std::make_shared<ConstantTexture>(roughness)), _ior(ior) {}
  Dielectric(std::shared_ptr<Texture> roughness_texture, float ior) : _roughness(roughness_texture), _ior(ior) {}
  virtual bool scatter(const Ray &iray, const HitRecord &hit, Vec3 &attenuation, Ray &scattered) const override;

public:
  std::shared_ptr<Texture> _roughness;
  float _ior;
};

class Metal : public Material {
public:
  Metal(const Vec3 &c, float r);
  Metal(std::shared_ptr<Texture> color_texture, std::shared_ptr<Texture> roughness_texture);

  virtual bool scatter(const Ray &iray, const HitRecord &hit, Vec3 &attenuation, Ray &scattered) const override;

public:
  std::shared_ptr<Texture> _color;
  std::shared_ptr<Texture> _roughness;
};

std::shared_ptr<Metal> create_mirror_material();
std::shared_ptr<Dielectric> create_lens_material();
std::shared_ptr<Lambertian> create_lambert_material();

#endif