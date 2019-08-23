#include "raytracer/integrators/BasicIntegrator.h"
#include "raytracer/materials/materials.h"
#include "raytracer/multithreading.h"
#include "raytracer/random_numbers.h"
#include "raytracer/shapes/BvhNode.h"

using namespace raytracer;

namespace {
  std::shared_ptr<BvhNode> build_bvh(std::vector<std::shared_ptr<Shape> > geoms, float t0, float t1) {
    auto copied_geom_list = geoms;
    return std::make_shared<BvhNode>(copied_geom_list.begin(), copied_geom_list.end(), t0, t1);
  }
} // internal namespace

BasicIntegrator::BasicIntegrator(std::shared_ptr<Image<float>> img, 
  size_t samples_per_pixel, 
  size_t max_ray_depth, 
  float t_begin, 
  float t_end)
: _image(img)
, _samples_per_pixel(samples_per_pixel)
, _max_ray_depth(max_ray_depth)
, _t_begin(t_begin)
, _t_end(t_end)
{

}

void BasicIntegrator::render(const Scene &scene) {
  _miss_color = scene._background_color;

  auto bvh = build_bvh(scene._children, _t_begin, _t_end);

  RT_FOR((size_t)0, _image->_width, [this, &bvh, &scene](size_t x) -> void {
    for (size_t y = 0; y < _image->_height; y++) {
      Vec3 color(0, 0, 0);
      
      for (size_t sample=0; sample < _samples_per_pixel; sample++) {
        float u = float(x + random_number()) / float(_image->_width);
        float v = float(y + random_number()) / float(_image->_height);
      
        Ray r = scene._camera->get_ray(u, v, _t_begin, _t_end);
        color += Li(r, *bvh.get(), 0);
      }
      
      color /= float(_samples_per_pixel);

      // a little y-flip
      _image->set(x, _image->_height - y - 1, color);
    }
  });
}

Vec3 BasicIntegrator::Li(const Ray &r, Shape &world, size_t depth) {
  HitRecord record;
  if (world.hit(r, 0.001f, std::numeric_limits<float>::max(), record)) {
    ScatterRecord srec;
    Vec3 emitted = record.material->emit(r, record);
    if (depth < _max_ray_depth && record.material->scatter(r, record, srec)) {
      if (srec._is_specular) {
        return (srec._attenuation.array() * Li(srec._specular_ray, world, depth + 1).array()).matrix();
      }

      // ObjectPDF plight(light_shape, record.p);
      // MixturePDF pdf(&plight, srec.pdf.get());
      PDF &pdf = *srec._pdf;
      Ray scattered = Ray(record.p, pdf.generate(), r.time());
      float pdf_val = pdf.value(scattered.direction());

      // debug:
      // return normal_to_color(record.normal);
      return emitted + 
        // record.material->scatter_pdf(r, record, scattered) *
        (
          srec._attenuation.array() *
          Li(scattered, world, depth+1).array()
        ).matrix()
        ;// / pdf_val;
    }
    return emitted;
  } else {
    return _miss_color;
  }
}