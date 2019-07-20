#include "raytracer/BvhNode.h"
#include "raytracer/random_numbers.h"
#include <algorithm>
#include <functional>

using comparison_fn_t = std::function<bool(const std::shared_ptr<IHitable> &, const std::shared_ptr<IHitable> &)>;

comparison_fn_t box_compare(int axis) {
  auto fn = [axis](const std::shared_ptr<IHitable> &a, const std::shared_ptr<IHitable> &b) -> bool {
    AxisAlignedBoundingBox left, right;
    if (!a->bounding_box(0, 0, left) || !b->bounding_box(0, 0, right))
      std::cerr << "no bounding box in BvhNode constructor!";
    
    return (left.min()[axis] - right.min()[axis] < 0.0f);
  };

  return fn;
}

BvhNode::BvhNode(BvhNode::iterator_t list_begin, BvhNode::iterator_t list_end, float t0, float t1) 
{
  int axis = int(3 * random_number());
  comparison_fn_t comparator = box_compare(axis);
  std::sort(list_begin, list_end, comparator);

  int n = std::distance(list_begin, list_end);

  if (n == 1)
    _left = _right = *list_begin;
  else if (n == 2) {
    _left = *list_begin;
    _right = *(list_begin + 1);
  } else {
    size_t half_it = (list_end - list_begin)/2;
    _left  = std::make_shared<BvhNode>(list_begin, list_begin + half_it, t0, t1);
    _right = std::make_shared<BvhNode>(list_begin + half_it, list_end, t0, t1);
  }

  AxisAlignedBoundingBox box_left, box_right;
  if (!_left->bounding_box(t0, t1, box_left) || !_right->bounding_box(t0, t1, box_right))
    std::cerr << "no bounding box in BvhNode constructor!";
  _aabb = box_left.combine(box_right);
}

bool BvhNode::hit(const Ray& r, float t_min, float t_max, HitRecord &record) const {
  if (!_aabb.hit(r, t_min, t_max)) return false;

  HitRecord left_hit, right_hit;
  bool is_left_hit = _left->hit(r, t_min, t_max, left_hit);
  bool is_right_hit = _right->hit(r, t_min, t_max, right_hit);
  if (is_left_hit && is_right_hit) {
    if (left_hit.t < right_hit.t) {
      record = left_hit;
    } else {
      record = right_hit;
    }
    return true;
  } else if (is_left_hit) {
    record = left_hit;
    return true;
  } else if (is_right_hit) {
    record = right_hit;
    return true;
  }

  return false;
}

bool BvhNode::bounding_box(float t0, float t1, AxisAlignedBoundingBox& aabb) const {
  aabb = _aabb;
  return true;
}