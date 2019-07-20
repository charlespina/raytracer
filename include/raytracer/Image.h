#ifndef RT_IMAGE_H
#define RT_IMAGE_H

#include <vector>

#define RT_IMG_CHANNELS 3

template<typename T>
struct Image {
  Image(size_t width, size_t height)
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

  inline size_t index(size_t x, size_t y) const {
    return  x * RT_IMG_CHANNELS + y * _width * RT_IMG_CHANNELS;
  }

  size_t _width;
  size_t _height;
  std::vector<T> _data;
};

#endif