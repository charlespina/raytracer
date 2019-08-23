#include "raytracer/shapes/BvhNode.h"
#include "raytracer/random_numbers.h"
#include <algorithm>
#include <functional>

namespace raytracer {

using comparison_fn_t = std::function<bool(const std::shared_ptr<Shape> &, const std::shared_ptr<Shape> &)>;

comparison_fn_t box_compare(int axis) {
  auto fn = [axis](const std::shared_ptr<Shape> &a, const std::shared_ptr<Shape> &b) -> bool {
    AxisAlignedBoundingBox left, right;
    left.setEmpty();
    right.setEmpty();
    if (!a->bounding_box(0, 0, left) || !b->bounding_box(0, 0, right))
      std::cerr << "no bounding box in BvhNode constructor!";
    
    return (left.min()[axis] - right.min()[axis]) < 0.0f;
  };

  return fn;
}

BvhNode::BvhNode(BvhNode::iterator_t list_begin, BvhNode::iterator_t list_end, float t0, float t1) 
{
  int axis = int(3 * random_number());
  comparison_fn_t comparator = box_compare(axis);
  std::sort(list_begin, list_end, comparator);

  size_t n = std::distance(list_begin, list_end);

  if (n == 1) {
    _left = _right = *list_begin;
  } else if (n == 2) {
    _left = *list_begin;
    _right = *(list_begin + 1);
  } else {
    size_t half_it = (list_end - list_begin)/2;
    _left  = std::make_shared<BvhNode>(list_begin, list_begin + half_it + 1, t0, t1);
    _right = std::make_shared<BvhNode>(list_begin + half_it, list_end, t0, t1);
  }

  AxisAlignedBoundingBox box_left, box_right;
  box_left.setEmpty();
  box_right.setEmpty();
  if (!_left->bounding_box(t0, t1, box_left) || !_right->bounding_box(t0, t1, box_right))
    std::cerr << "no bounding box in BvhNode constructor!";
  _aabb = box_left.merged(box_right);
}

bool BvhNode::hit(const Ray& r, float t_min, float t_max, HitRecord &record) const {
  if (!aabb_hit(_aabb, r, t_min, t_max)) return false;

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

size_t BvhNode::count_left() const {
  size_t result = 0;
  if (auto left_bvh = std::dynamic_pointer_cast<BvhNode>(_left)) result += left_bvh->count();
  else if (_left != nullptr)
    result += 1;
  return result;
}

size_t BvhNode::count_right() const {
  size_t result = 0;
  if (auto right_bvh = std::dynamic_pointer_cast<BvhNode>(_right)) result += right_bvh->count();
  else if (_right != nullptr)
    result += 1;
  return result;
}

size_t BvhNode::count() const {
  size_t result = 0;
  result += count_left();
  result += count_right();
  return result;
}

} // raytracer