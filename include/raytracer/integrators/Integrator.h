#pragma once

#include "raytracer/Scene.h"

namespace raytracer {

class Integrator {
  virtual void render(const Scene &scene) = 0;
};

}