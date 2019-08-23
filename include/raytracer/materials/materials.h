#ifndef RT_MATERIALS_H
#define RT_MATERIALS_H

#include "raytracer/materials/SurfaceInteraction.h"
#include "raytracer/random_numbers.h"
#include "raytracer/Ray.h"
#include "raytracer/materials/pdfs.h"
#include "raytracer/textures/textures.h"
#include <memory>

namespace raytracer {

class Material {
public:
  virtual bool scatter(const Ray &iray, SurfaceInteraction &inout_ix) const = 0;
  virtual float scatter_pdf(const Ray &iray, const SurfaceInteraction &ix) const { return 0; }
  virtual Vec3 emit(const Ray &iray, const SurfaceInteraction &ix) const { return Vec3(0, 0, 0); }
};

class Lambertian : public Material {
public:
  Lambertian(const Vec3 &albedo) : _albedo(std::make_shared<ConstantTexture>(albedo)) {}
  Lambertian(std::shared_ptr<Texture> albedo_texture) : _albedo(albedo_texture) {}
  virtual bool scatter(const Ray &iray, SurfaceInteraction &inout_ix) const override;
  virtual float scatter_pdf(const Ray &iray, const SurfaceInteraction &ix) const override;

public:
  std::shared_ptr<Texture> _albedo;
};

class Dielectric : public Material {
public:
  Dielectric(float roughness, float ior) : _roughness(std::make_shared<ConstantTexture>(roughness)), _ior(ior) {}
  Dielectric(std::shared_ptr<Texture> roughness_texture, float ior) : _roughness(roughness_texture), _ior(ior) {}
  virtual bool scatter(const Ray &iray, SurfaceInteraction &inout_ix) const override;

public:
  std::shared_ptr<Texture> _roughness;
  float _ior;
};

class Metal : public Material {
public:
  Metal(const Vec3 &c, float r);
  Metal(std::shared_ptr<Texture> color_texture, std::shared_ptr<Texture> roughness_texture);

  virtual bool scatter(const Ray &iray, SurfaceInteraction &inout_ix) const override;

public:
  std::shared_ptr<Texture> _color;
  std::shared_ptr<Texture> _roughness;
};

class DiffuseLight : public Material {
public:
  DiffuseLight(std::shared_ptr<Texture> emissive) : _emissive(emissive) {}
  
  virtual bool scatter(const Ray &iray, SurfaceInteraction &inout_ix) const override;
  virtual Vec3 emit(const Ray &iray, const SurfaceInteraction &ix) const override;

public:
  std::shared_ptr<Texture> _emissive;

};

class Isotropic : public Material {
public:
  Isotropic(std::shared_ptr<Texture> texture) : _albedo(texture) {}
  virtual bool scatter(const Ray &iray, SurfaceInteraction &hit) const override {
    hit.is_specular = true;
    hit.wo = random_in_unit_sphere().normalized();
    hit.attenuation = _albedo->sample_color(hit.texcoord.x(), hit.texcoord.y(), hit.p);
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
