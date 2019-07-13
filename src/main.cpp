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


vec3 color(const ray &r) {
  vec3 unit_direction = unit_vector(r.direction());
  float t = 0.5f * (unit_direction.y() + 1.0f);
  return (1.0f - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7f, 1.0f);
}

int main(char **argv) {
    image<float> img(RT_IMG_WIDTH, RT_IMG_HEIGHT);

    vec3 v;
    for (size_t x = 0; x < img._width; x++) {
      for (size_t y = 0; y < img._height; y++) {
        v.e[0] = (float)x / img._width;
        v.e[1] = (float)y / img._height;
        v.e[2] = 0;
        img.set(x, img._height - y - 1, v);
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