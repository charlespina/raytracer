#pragma once

#include "raytracer/materials/pdfs.h"
#include "raytracer/shapes/Shape.h"

namespace raytracer {

class ObjectPDF : public PDF {
public:
  ObjectPDF(std::shared_ptr<Shape> obj, const Vec3 &origin) : _origin(origin), _object(obj) {}

  float value(const Vec3 &dir) const override {
    return _object->pdf_value(_origin, dir);
  }

  Vec3 generate() const {
    return _object->random(_origin);
  }

public:
  Vec3 _origin;
  std::shared_ptr<Shape> _object;

};

}