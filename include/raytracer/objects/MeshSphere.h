#ifndef RT_MESHSPHERE_H_
#define RT_MESHSPHERE_H_

#include "raytracer/objects/Mesh.h"

namespace raytracer {

class MeshSphere : public Mesh {
public:
  MeshSphere(float radius, size_t segments, size_t sections, std::shared_ptr<Material> material);
  virtual ~MeshSphere() {}
};

}

#endif