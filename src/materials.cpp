#include "raytracer/materials.h"

namespace {
  bool refract(const vec3 &v, const vec3 &n, float ni_over_nt, vec3 &refracted) {
    vec3 uv = unit_vector(v);
    float dt = dot(uv, n);
    float discriminant = 1.0 - ni_over_nt*ni_over_nt*(1.0f-dt*dt);
    if (discriminant <= 0) return false;
    refracted = ni_over_nt * (uv - n * dt) - n * sqrtf(discriminant);
    return true;
  }

  inline vec3 roughness_lobe(float roughness) {
    return roughness * roughness * random_in_unit_sphere();
  }

  float schlick(float cosine, float ior) {
    float r0 = (1.0f - ior) / (1.0f + ior);
    r0 = r0 * r0;
    return r0 + (1.0f - r0) * powf((1.0f - cosine), 5.0f);
  }
} // internal namespace


bool Lambertian::scatter(const ray &iray, const HitRecord &hit, vec3 &attenuation, ray &scattered) const {
  float ndv = dot(iray.direction(), hit.normal);
  float f0 = 0.08f;
  
  float P_reflect = schlick(abs(ndv), 1.0f + f0);

  if (random_number() < P_reflect) {
    vec3 reflected = reflect(iray.direction(), hit.normal);
    scattered = ray(hit.p, reflected, iray.time());
    attenuation = vec3(1.0f, 1.0f, 1.0f); // _albedo;
  } else {
    // diffuse scatter - scatter in random direction
    scattered = ray(hit.p, hit.normal + random_in_unit_sphere(), iray.time());
    attenuation = _albedo;
  }
  return true;
}

bool Dielectric::scatter(const ray &iray, const HitRecord &hit, vec3 &attenuation, ray &scattered) const {
  vec3 outward_normal;
  vec3 reflected = reflect(iray.direction(), hit.normal);
  float ni_over_nt;
  attenuation = vec3(1.0, 1.0, 1.0);
  vec3 refracted;
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

  if (random_number() < P_reflect) {
    scattered = ray(hit.p, reflected + roughness_lobe(_roughness), iray.time());
  } else {
    scattered = ray(hit.p, refracted + roughness_lobe(_roughness), iray.time()); 
  }

  return true;
}

std::shared_ptr<Lambertian> create_lambert_material() {
  return std::make_shared<Lambertian>(vec3(0.5f, 0.5f, 0.5f));
}

std::shared_ptr<Metal> create_mirror_material() {
  return std::make_shared<Metal>(vec3(1.0f, 1.0f, 1.0f), 0.0f);
}

std::shared_ptr<Dielectric> create_lens_material() {
  return std::make_shared<Dielectric>(0.0f, 1.0f);
}

bool Metal::scatter(const ray &iray, const HitRecord &hit, vec3 &attenuation, ray &scattered) const {
  vec3 reflected = reflect(unit_vector(iray.direction()), hit.normal);
  scattered = ray(hit.p, reflected + roughness_lobe(_roughness), iray.time());
  attenuation = _color;
  return ((dot(scattered.direction(), hit.normal)) > 0);
}