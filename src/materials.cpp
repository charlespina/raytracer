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
    return roughness * random_in_unit_sphere();
  }
} // internal namespace


bool Lambertian::scatter(const ray &iray, const HitRecord &hit, vec3 &attenuation, ray &scattered) const {
  vec3 target = hit.p + hit.normal + random_in_unit_sphere();
  scattered = ray(hit.p, target - hit.p);
  attenuation = _albedo;
  return true;
}

bool Dielectric::scatter(const ray &iray, const HitRecord &hit, vec3 &attenuation, ray &scattered) const {
  vec3 outward_normal;
  vec3 reflected = reflect(iray.direction(), hit.normal);
  float ni_over_nt;
  attenuation = vec3(1.0, 1.0, 1.0);
  vec3 refracted;
  if (dot(iray.direction(), hit.normal) > 0) {
    outward_normal = -hit.normal;
    ni_over_nt = _ior;
  } else {
    outward_normal = hit.normal;
    ni_over_nt = 1.0f / _ior;
  }

  if (refract(iray.direction(), outward_normal, ni_over_nt, refracted)) {
    scattered = ray(hit.p, refracted + roughness_lobe(_roughness));
  } else {
    scattered = ray(hit.p, reflected + roughness_lobe(_roughness));
  }

  return true;
}

bool Metal::scatter(const ray &iray, const HitRecord &hit, vec3 &attenuation, ray &scattered) const {
  vec3 reflected = reflect(unit_vector(iray.direction()), hit.normal);
  scattered = ray(hit.p, reflected + roughness_lobe(_roughness));
  attenuation = _color;
  return ((dot(scattered.direction(), hit.normal)) > 0);
}