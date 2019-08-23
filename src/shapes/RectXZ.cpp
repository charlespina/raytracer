#include "raytracer/shapes/RectXZ.h"

using namespace raytracer;

RectXZ::RectXZ(float width, float height, std::shared_ptr<Material> mat)
: _width(width)
, _height(height)
, _x0(-width/2.0f)
, _x1(width/2.0f)
, _z0(-height/2.0f)
, _z1(height/2.0f)
, _material(mat)
{
}

bool RectXZ::hit(const Ray& r, float t_min, float t_max, SurfaceInteraction &record) const {
  float y = 0;
  float t = (y - r.origin().y()) / r.direction().y();
  if (t < t_min || t > t_max)
    return false;
  
  Vec3 p = r.origin() + t * r.direction();

  if (p.x() < _x0 || p.x() > _x1 
   || p.z() < _z0 || p.z() > _z1)
    return false;

  record.texcoord = TexCoord(
    (p.x() - _x0) / (_x1 - _x0),
    (p.z() - _z0) / (_z1 - _z0));
  
  record.t = t;
  record.material = _material.get();
  record.normal = Vec3(0, 1.0f, 0);
  return true;
}

bool RectXZ::bounding_box(float t0, float t1, AxisAlignedBoundingBox& aabb) const {
  AxisAlignedBoundingBox box(Vec3(_x0, 0, _z0), Vec3(_x1, 0, _z1));
  aabb = box;
  return true;
}