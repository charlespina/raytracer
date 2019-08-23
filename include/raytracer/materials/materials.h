#ifndef RT_MATERIALS_H
#define RT_MATERIALS_H

#include "raytracer/HitRecord.h"
#include "raytracer/random_numbers.h"
#include "raytracer/Ray.h"
#include "raytracer/materials/pdfs.h"
#include "raytracer/textures/textures.h"
#include <memory>

namespace raytracer {

struct ScatterRecord {
  Ray _specular_ray;
  bool _is_specular;
  Vec3 _attenuation;
  std::shared_ptr<PDF> _pdf;
};

class Material {
public:
  virtual bool scatter(const Ray &iray, const HitRecord &hit, ScatterRecord &out_scatter) const = 0;
  virtual float scatter_pdf(const Ray &iray, const HitRecord &hit, const Ray &scattered) const { return 0; }
  virtual Vec3 emit(const Ray &iray, const HitRecord &hit) const { return Vec3(0, 0, 0); }
};

class Lambertian : public Material {
public:
  Lambertian(const Vec3 &albedo) : _albedo(std::make_shared<ConstantTexture>(albedo)) {}
  Lambertian(std::shared_ptr<Texture> albedo_texture) : _albedo(albedo_texture) {}
  virtual bool scatter(const Ray &iray, const HitRecord &hit, ScatterRecord &out_scatter) const override;
  virtual float scatter_pdf(const Ray &iray, const HitRecord &hit, const Ray &scattered) const override;

public:
  std::shared_ptr<Texture> _albedo;
};

class Dielectric : public Material {
public:
  Dielectric(float roughness, float ior) : _roughness(std::make_shared<ConstantTexture>(roughness)), _ior(ior) {}
  Dielectric(std::shared_ptr<Texture> roughness_texture, float ior) : _roughness(roughness_texture), _ior(ior) {}
  virtual bool scatter(const Ray &iray, const HitRecord &hit, ScatterRecord &out_scatter) const override;

public:
  std::shared_ptr<Texture> _roughness;
  float _ior;
};

class Metal : public Material {
public:
  Metal(const Vec3 &c, float r);
  Metal(std::shared_ptr<Texture> color_texture, std::shared_ptr<Texture> roughness_texture);

  virtual bool scatter(const Ray &iray, const HitRecord &hit, ScatterRecord &out_scatter) const override;

public:
  std::shared_ptr<Texture> _color;
  std::shared_ptr<Texture> _roughness;
};

class DiffuseLight : public Material {
public:
  DiffuseLight(std::shared_ptr<Texture> emissive) : _emissive(emissive) {}
  
  virtual bool scatter(const Ray &iray, const HitRecord &hit, ScatterRecord &out_scatter) const override;
  virtual Vec3 emit(const Ray &iray, const HitRecord &hit) const override;

public:
  std::shared_ptr<Texture> _emissive;

};

class Isotropic : public Material {
public:
  Isotropic(std::shared_ptr<Texture> texture) : _albedo(texture) {}
  virtual bool scatter(const Ray &iray, const HitRecord &hit, ScatterRecord &out_scatter) const override {
    out_scatter._is_specular = true;
    out_scatter._specular_ray = Ray(hit.p, random_in_unit_sphere().normalized(), iray.time());
    out_scatter._attenuation = _albedo->sample_color(hit.texcoord.x(), hit.texcoord.y(), hit.p);
    return true; 
  }
public:
  std::shared_ptr<Texture> _albedo;
};

std::shared_ptr<Metal> create_mirror_material();
std::shared_ptr<Dielectric> create_lens_material();
std::shared_ptr<Lambertian> create_lambert_material();

} // raytracer

#endif
