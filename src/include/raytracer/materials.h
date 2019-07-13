#ifndef RT_MATERIALS_H
#define RT_MATERIALS_H

#include "raytracer/HitRecord.h"
#include "raytracer/materials.h"
#include "raytracer/random_numbers.h"
#include "raytracer/ray.h"

class Material {
public:
  virtual bool scatter(const ray &iray, const HitRecord &hit, vec3 &atten, ray &scattered) const = 0;
};

class Lambertian : public Material {
public:
  Lambertian(const vec3 &albedo) : _albedo(albedo) {}
  virtual bool scatter(const ray &iray, const HitRecord &hit, vec3 &attenuation, ray &scattered) const override;

public:
  vec3 _albedo;
};

class Dielectric : public Material {
public:
  Dielectric(float roughness, float ior) : _roughness(roughness), _ior(ior) {}
  virtual bool scatter(const ray &iray, const HitRecord &hit, vec3 &attenuation, ray &scattered) const override;

public:
  float _roughness;
  float _ior;
};

class Metal : public Material {
public:
  Metal(const vec3 &color, float roughness) 
  : _color(color)
  , _roughness(roughness) {}
  
  virtual bool scatter(const ray &iray, const HitRecord &hit, vec3 &attenuation, ray &scattered) const override;
  
  vec3 _color;
  float _roughness;
};

#endif