#include "raytracer/materials.h"
#include "raytracer/OrthoNormalBasis.h"

namespace raytracer {

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


bool Lambertian::scatter(const Ray &iray, const HitRecord &hit, ScatterRecord &srec) const {
  float u = hit.texcoord.u();
  float v = hit.texcoord.v();
  srec._is_specular = false;
  srec._attenuation = _albedo->sample_color(u, v, hit.p);
  srec._pdf = std::make_shared<CosinePDF>(hit.normal);
  return true;
}

float Lambertian::scatter_pdf(const Ray &iray, const HitRecord &hit, const Ray &scattered) const {
  float cosine = dot(hit.normal, scattered.direction());

  if (cosine < 0) return 0;
  return cosine / RT_PI;
}

bool Dielectric::scatter(const Ray &iray, const HitRecord &hit, ScatterRecord &srec) const {
  srec._is_specular = true;
  srec._pdf = nullptr;
  srec._attenuation = Vec3(1, 1, 1);

  Vec3 outward_normal;
  Vec3 reflected = reflect(iray.direction(), hit.normal);
  float ni_over_nt;
  Vec3 refracted;
  float P_reflect;
  float cosine;
  if (dot(iray.direction(), hit.normal) > 0) {
    outward_normal = -hit.normal;
    ni_over_nt = _ior;
    cosine = _ior * dot(iray.direction(), hit.normal) / iray.direction().norm();
  } else {
    outward_normal = hit.normal;
    ni_over_nt = 1.0f / _ior;
    cosine = -dot(iray.direction(), hit.normal) / iray.direction().norm();
  }

  if (refract(iray.direction(), outward_normal, ni_over_nt, refracted)) {
    P_reflect = schlick(cosine, _ior);
  } else {
    P_reflect = 1.0f;
  }

  float u = 0, v = 0;
  if (random_number() < P_reflect) {
    srec._specular_ray = Ray(hit.p, reflected + roughness_lobe(_roughness->sample_scalar(u, v, hit.p)), iray.time());
  } else {
    srec._specular_ray = Ray(hit.p, refracted + roughness_lobe(_roughness->sample_scalar(u, v, hit.p)), iray.time()); 
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
, _roughness(std::make_shared<ConstantTexture>(roughness))
{}

Metal::Metal(std::shared_ptr<Texture> color, std::shared_ptr<Texture> roughness)
: _color(color)
, _roughness(roughness)
{}

bool Metal::scatter(const Ray &iray, const HitRecord &hit, ScatterRecord &srec) const {
  Vec3 reflected = reflect(unit_vector(iray.direction()), hit.normal);
  float u = 0, v = 0;
  srec._specular_ray = Ray(hit.p, reflected + roughness_lobe(_roughness->sample_scalar(u, v, hit.p)), iray.time());
  srec._attenuation = _color->sample_color(u, v, hit.p);
  srec._is_specular = true;
  return true;
}

bool DiffuseLight::scatter(const Ray &iray, const HitRecord &hit, ScatterRecord &srec) const {
  return false;
}

Vec3 DiffuseLight::emit(const Ray &iray, const HitRecord &hit) const {
  float t = hit.t;
  // float t = (hit.p - iray.origin()).norm();
  float falloff = 1.0f; // / (t * t); 
  return falloff * _emissive->sample_color(hit.texcoord.u(), hit.texcoord.v(), hit.p);
}

} // raytracer