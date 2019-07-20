#ifndef RT_MATERIALS_H
#define RT_MATERIALS_H

#include "raytracer/HitRecord.h"
#include "raytracer/random_numbers.h"
#include "raytracer/Ray.h"
#include <memory>

class Material {
public:
  virtual bool scatter(const Ray &iray, const HitRecord &hit, Vec3 &atten, Ray &scattered) const = 0;
};

class Lambertian : public Material {
public:
  Lambertian(const Vec3 &albedo) : _albedo(albedo) {}
  virtual bool scatter(const Ray &iray, const HitRecord &hit, Vec3 &attenuation, Ray &scattered) const override;

public:
  Vec3 _albedo;
};

class Dielectric : public Material {
public:
  Dielectric(float roughness, float ior) : _roughness(roughness), _ior(ior) {}
  virtual bool scatter(const Ray &iray, const HitRecord &hit, Vec3 &attenuation, Ray &scattered) const override;

public:
  float _roughness;
  float _ior;
};

class Metal : public Material {
public:
  Metal(const Vec3 &color, float roughness) 
  : _color(color)
  , _roughness(roughness) {}
  
  virtual bool scatter(const Ray &iray, const HitRecord &hit, Vec3 &attenuation, Ray &scattered) const override;
  
  Vec3 _color;
  float _roughness;
};

std::shared_ptr<Metal> create_mirror_material();
std::shared_ptr<Dielectric> create_lens_material();
std::shared_ptr<Lambertian> create_lambert_material();

#endif