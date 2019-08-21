#ifndef RT_RANDOMNUMBERGENERATOR_H
#define RT_RANDOMNUMBERGENERATOR_H

#include "raytracer/Vec3.h"

namespace raytracer {

float random_number();

Vec3 random_in_unit_sphere();

Vec3 random_on_unit_sphere();

Vec3 random_in_unit_disk();

Vec3 random_cosine_direction();

} // raytracer

#endif