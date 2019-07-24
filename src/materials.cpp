#include "raytracer/materials.h"

namespace {
  bool refract(const Vec3 &v, const Vec3 &n, float ni_over_nt, Vec3 &refracted) {
    Vec3 uv = unit_vector(v);
    float dt = dot(uv, n);
    float discriminant = 1.0f - ni_over_nt*ni_over_nt*(1.0f-dt*dt);
    if (discriminant <= 0) return false;
    refracted = ni_over_nt * (uv - n * dt) - n * sqrtf(discriminant);
    return true;
  }

  inline Vec3 roughness_lobe(float roughness) {
    return roughness * roughness * random_in_unit_sphere();
  }

  float schlick(float cosine, float ior) {
    float r0 = (1.0f - ior) / (1.0f + ior);
    r0 = r0 * r0;
    return r0 + (1.0f - r0) * powf((1.0f - cosine), 5.0f);
  }
} // internal namespace


bool Lambertian::scatter(const Ray &iray, const HitRecord &hit, Vec3 &attenuation, Ray &scattered) const {
  float ndv = dot(iray.direction(), hit.normal);
  float f0 = 0.08f;
  
  float P_reflect = schlick(abs(ndv), 1.0f + f0);
  float u = hit.texcoord.u();
  float v = hit.texcoord.v();

  if (random_number() < P_reflect) {
    Vec3 reflected = reflect(iray.direction(), hit.normal);
    scattered = Ray(hit.p, reflected, iray.time());
    attenuation = Vec3(1.0f, 1.0f, 1.0f); // _albedo;
  } else {
    // diffuse scatter - scatter in random direction
    scattered = Ray(hit.p, hit.normal + random_in_unit_sphere(), iray.time());
    attenuation = _albedo->sample_color(u, v, hit.p);
  }
  return true;
}

bool Dielectric::scatter(const Ray &iray, const HitRecord &hit, Vec3 &attenuation, Ray &scattered) const {
  Vec3 outward_normal;
  Vec3 reflected = reflect(iray.direction(), hit.normal);
  float ni_over_nt;
  attenuation = Vec3(1.0, 1.0, 1.0);
  Vec3 refracted;
  float P_reflect;
  float cosine;
  if (dot(iray.direction(), hit.normal) > 0) {
    outward_normal = -hit.normal;
    ni_over_nt = _ior;
    cosine = _ior * dot(iray.direction(), hit.normal) / iray.direction().length();
  } else {
    outward_normal = hit.normal;
    ni_over_nt = 1.0f / _ior;
    cosine = -dot(iray.direction(), hit.normal) / iray.direction().length();
  }

  if (refract(iray.direction(), outward_normal, ni_over_nt, refracted)) {
    P_reflect = schlick(cosine, _ior);
  } else {
    P_reflect = 1.0f;
  }

  float u = 0, v = 0;
  if (random_number() < P_reflect) {
    scattered = Ray(hit.p, reflected + roughness_lobe(_roughness->sample_scalar(u, v, hit.p)), iray.time());
  } else {
    scattered = Ray(hit.p, refracted + roughness_lobe(_roughness->sample_scalar(u, v, hit.p)), iray.time()); 
  }

  return true;
}

std::shared_ptr<Lambertian> create_lambert_material() {
  return std::make_shared<Lambertian>(Vec3(0.5f, 0.5f, 0.5f));
}

std::shared_ptr<Metal> create_mirror_material() {
  return std::make_shared<Metal>(Vec3(1.0f, 1.0f, 1.0f), 0.0f);
}

std::shared_ptr<Dielectric> create_lens_material() {
  return std::make_shared<Dielectric>(0.0f, 1.0f);
}

Metal::Metal(const Vec3 &color, float roughness)
: _color(std::make_shared<ConstantTexture>(color))
, _roughness(std::make_shared<ConstantTexture>(Vec3(roughness)))
{}

Metal::Metal(std::shared_ptr<Texture> color, std::shared_ptr<Texture> roughness)
: _color(color)
, _roughness(roughness)
{}

bool Metal::scatter(const Ray &iray, const HitRecord &hit, Vec3 &attenuation, Ray &scattered) const {
  Vec3 reflected = reflect(unit_vector(iray.direction()), hit.normal);
  float u = 0, v = 0;
  scattered = Ray(hit.p, reflected + roughness_lobe(_roughness->sample_scalar(u, v, hit.p)), iray.time());
  attenuation = _color->sample_color(u, v, hit.p);
  return ((dot(scattered.direction(), hit.normal)) > 0);
}

bool DiffuseLight::scatter(const Ray &iray, const HitRecord &hit, Vec3 &attenuation, Ray &scattered) const {
  return false;
}

Vec3 DiffuseLight::emit(const HitRecord &hit) const { 
  return _emission->sample_color(hit.texcoord.u(), hit.texcoord.v(), hit.p);
}