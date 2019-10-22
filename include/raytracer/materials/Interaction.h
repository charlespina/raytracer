#pragma once

#include "raytracer/Vec3.h"

namespace raytracer {

struct Interaction {
  float t;
  Vec3 p;
  Vec3 normal;
  Vec3 wo;
};

}