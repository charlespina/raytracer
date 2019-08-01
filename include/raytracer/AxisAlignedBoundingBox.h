#ifndef RT_AXISALIGNEDBOUNDINGBOX_H
#define RT_AXISALIGNEDBOUNDINGBOX_H

#include "raytracer/Vec3.h"
#include "raytracer/Ray.h"
#include "Eigen/Dense"

using AxisAlignedBoundingBox = Eigen::AlignedBox3f;

bool aabb_hit(const AxisAlignedBoundingBox &box, const Ray& r, float tmin, float tmax);

#endif // header guard