#include <iostream>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

#include "raytracer/vec3.h"
#include "raytracer/ray.h"


#define RT_IMG_WIDTH 400
#define RT_IMG_HEIGHT 200
#define RT_IMG_CHANNELS 3

struct hit_record {
  float t;
  vec3 p;
  vec3 normal;
};

class hitable {
public:
  virtual bool hit(const ray& r, float t_min, float t_max, hit_record &record) const = 0;
};

class sphere : public hitable {
public:
  sphere() = default;
  sphere(vec3 center, float r)
  : _center(center)
  , _radius(r)
  {}

  virtual bool hit(const ray& r, float tmin, float tmax, hit_record &record) const override;
  
  vec3 _center;
  float _radius;
};

bool sphere::hit(const ray& r, float tmin, float tmax, hit_record &record) const {
  vec3 oc = r.origin() - _center;
  float a = dot(r.direction(), r.direction());
  float b = 2.0f * dot(oc, r.direction());
  float c = dot(oc, oc) - _radius * _radius;
  float discriminant = b*b - a*c;

  if (discriminant < 0) return false;

  float t = (-b - sqrtf(discriminant)) / (2.0f * a);

  t = 

}

class scene : public hitable {
public:
  virtual bool hit(const ray& r, float tmin, float tmax, hit_record &record) const override;
  std::vector<std::shared_ptr<hitable>> geometries;
};

bool scene::hit(const ray& r, float t_min, float t_max, hit_record &record) const {
  bool hit_anything = false;
  float closest_so_far = t_max;
  hit_record temp_record;
  for (const auto &geom : geometries) {
    if (geom->hit(r, t_min, closest_so_far, temp_record)) {
      hit_anything = true;
      closest_so_far = temp_record.t;
      record = temp_record;
    }
  }
  return hit_anything;
}

template<typename T>
struct image {
  image(size_t width, size_t height) 
    : _width(width)
    , _height(height)
    , _data(RT_IMG_CHANNELS * width * height) 
  {}

  void set(size_t x, size_t y, const vec3 &v) {
    size_t px = index(x, y);
    _data[px + 0] = v.r();
    _data[px + 1] = v.g();
    _data[px + 2] = v.b();
  }

  void set(size_t x, size_t y, T r, T g, T b) {
    size_t px = index(x, y);
    _data[px + 0] = r;
    _data[px + 1] = g;
    _data[px + 2] = b;
  }

  void get(size_t x, size_t y, vec3 &v) {
    size_t px = index(x, y);
    v.e[0] = _data[px + 0];
    v.e[1] = _data[px + 1];
    v.e[2] = _data[px + 2];
  }

  size_t image::index(size_t x, size_t y) {
    return  x * RT_IMG_CHANNELS + y * _width * RT_IMG_CHANNELS;
  }

  size_t _width;
  size_t _height;
  std::vector<T> _data;
};

float hit_sphere(const vec3 &center, float radius, const ray &r) {
  vec3 oc = r.origin() - center;
  float a = dot(r.direction(), r.direction());
  float b = 2.0f * dot(oc, r.direction());
  float c = dot(oc, oc) - radius * radius;
  float discriminant = b*b - 4.0f*a*c;

  if (discriminant < 0) return -1.0f;
  return (-b - sqrtf(discriminant)) / (2.0f * a);
}

vec3 normal_to_color(const vec3 &n) {
  return 0.5f * (n + vec3(1.0f, 1.0f, 1.0f));
}

vec3 color(const ray &r) {
  vec3 sphere_pos(0, 0, -1);
  float t = hit_sphere(sphere_pos, 0.5f, r);
  if (t > 0.0f) {
    vec3 N = unit_vector(r.point_at_parameter(t) - sphere_pos);

    return normal_to_color(N);
  }
  vec3 unit_direction = unit_vector(r.direction());
  t = 0.5f * (unit_direction.y() + 1.0f);
  return (1.0f - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7f, 1.0f);
}

int main(char **argv) {
    image<float> img(RT_IMG_WIDTH, RT_IMG_HEIGHT);

    vec3 lower_left_corner(-2.0f, -1.0f, -1.0f);
    vec3 horizontal(4.0, 0.0, 0.0);
    vec3 vertical(0.0, 2.0, 0.0);
    vec3 origin(0, 0, 0);

    for (size_t x = 0; x < img._width; x++) {
      for (size_t y = 0; y < img._height; y++) {
        float u = (float)x / img._width;
        float v = (float)y / img._height;

        ray r(origin, lower_left_corner + u*horizontal + v*vertical);
        vec3 c = color(r);

        // a little y-flip
        img.set(x, img._height - y - 1, c);
      }
    }

    // convert to output format
    image<uint8_t> img_out(img._width, img._height);
    vec3 v_out;
    for (size_t x = 0; x < img._width; x++) {
      for (size_t y = 0; y < img._height; y++) {
        img.get(x, y, v_out);

        // tonemap
        v_out.e[0] = sqrtf(v_out.r());
        v_out.e[1] = sqrtf(v_out.g());
        v_out.e[2] = sqrtf(v_out.b());

        // remap from 0.0-1.0 to 0-255
        v_out *= 255.99f;
        img_out.set(
          x, y,
          (uint8_t)v_out.r(), 
          (uint8_t)v_out.g(), 
          (uint8_t)v_out.b()
        );
      }
    }

    stbi_write_png("./out.png", 
      img_out._width, 
      img_out._height, 
      RT_IMG_CHANNELS, 
      img_out._data.data(),
      img_out._width * RT_IMG_CHANNELS);
    
    std::cout << "OK\n";

    return 0;
}