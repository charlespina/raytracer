#ifndef RT_SCENE_H
#define RT_SCENE_H

#include "raytracer/Camera.h"
#include "raytracer/HitRecord.h"
#include "raytracer/objects/IObject.h"
#include "raytracer/objects/Group.h"
#include "raytracer/textures.h"
#include <vector>

namespace raytracer {

class Scene : public Group {
public:
  template<typename ObjectType, typename ... Args>
  void create_object(Args && ...args) {
    add_object(std::make_shared<ObjectType>(std::forward<Args>(args)...));
  }

  virtual ~Scene() {}
  
public:
  std::shared_ptr<Camera> _camera;
};

} // raytracer

#endif