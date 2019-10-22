#ifndef RT_PDFS_H_
#define RT_PDFS_H_

#include "raytracer/math.h"
#include "raytracer/shapes/Shape.h"
#include "raytracer/OrthoNormalBasis.h"
#include "raytracer/random_numbers.h"
#include "raytracer/Vec3.h"

namespace raytracer {

class PDF {
public:
  virtual float value(const Vec3 &dir) const = 0;
  virtual Vec3 generate() const = 0;
};

class CosinePDF : public PDF {
public:
  CosinePDF(const Vec3 &w);
  float value(const Vec3 &direction) const override;
  Vec3 generate() const override;

public:
  OrthoNormalBasis _basis;
};

class ObjectPDF : public PDF {
public:
  ObjectPDF(std::shared_ptr<Shape> obj, const Vec3 &origin) : _origin(origin), _object(obj) {}

  float value(const Vec3 &dir) const override {
    return _object->pdf_value(_origin, dir);
  }

  Vec3 generate() const override {
    return _object->random(_origin);
  }

public:
  Vec3 _origin;
  std::shared_ptr<Shape> _object;

};

class MixturePDF : public PDF {
public:
  MixturePDF(PDF *p0, PDF *p1) : _p0(p0), _p1(p1) {}

  float value(const Vec3 &direction) const override {
    return 0.5f * _p0->value(direction) + 0.5f * _p1->value(direction);
  }

  Vec3 generate() const override {
    if (random_number() < 0.5f)
      return _p0->generate();
    else
      return _p1->generate();
  }

public:
  PDF *_p0;
  PDF *_p1;
};

} // namespace

#endif // header guard