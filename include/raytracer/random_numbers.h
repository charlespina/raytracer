#ifndef RT_RANDOMNUMBERGENERATOR_H
#define RT_RANDOMNUMBERGENERATOR_H

#include "raytracer/Vec3.h"

namespace raytracer {

float random_number();

Vec3 random_in_unit_sphere();

Vec3 random_in_unit_disk();

} // raytracer

#endif