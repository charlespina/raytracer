#include "raytracer/objects/Group.h"
#include "raytracer/random_numbers.h"

using namespace raytracer;

bool Group::hit(const Ray& r, float t_min, float t_max, HitRecord &record) const {
  bool hit_anything = false;
  float closest_so_far = t_max;
  HitRecord temp_record;
  for (const auto &geom : _children) {
    if (geom->hit(r, t_min, closest_so_far, temp_record)) {
      hit_anything = true;
      closest_so_far = temp_record.t;
      record = temp_record;
    }
  }
  return hit_anything;
}

bool Group::bounding_box(float t0, float t1, AxisAlignedBoundingBox& aabb) const {
  if (_children.empty()) return false;

  AxisAlignedBoundingBox temp_aabb;
  temp_aabb.setEmpty();
  if (!_children[0]->bounding_box(t0, t1, temp_aabb)) {
    return false;
  }

  aabb = temp_aabb;
  bool all_valid = std::all_of(_children.begin(), _children.end(), [t0, t1, &temp_aabb, &aabb](const auto &geom) -> bool {
    bool result = geom->bounding_box(t0, t1, temp_aabb);
    if (result) aabb.extend(temp_aabb);
    return result;
  });

  return all_valid;
}

float Group::pdf_value(const Vec3 &o, const Vec3 &dir) const {
  float weight = 1.0f/_children.size();
  float sum = 0.0f;
  for (const auto &child : _children) {
    sum += weight * child->pdf_value(o, dir);
  }
  return sum;
}

Vec3 Group::random(const Vec3 &o) const {
  size_t idx = size_t(random_number() * _children.size());
  return _children[idx]->random(o);
}