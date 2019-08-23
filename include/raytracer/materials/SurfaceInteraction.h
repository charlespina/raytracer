#pragma once

#include "raytracer/materials/Interaction.h"
#include "raytracer/TexCoord.h"
#include <memory>

namespace raytracer {

class PDF;
class Material;

struct SurfaceInteraction : public Interaction {
  TexCoord texcoord;
  Material *material;
  bool is_specular;
  Vec3 attenuation;
  std::shared_ptr<PDF> pdf;
};

}