#ifndef RT_IMAGE_H
#define RT_IMAGE_H

#include <vector>
#include <cassert>
#include "raytracer/Vec3.h"

#define RT_IMG_CHANNELS 3

template<typename T>
struct Image {
  Image(size_t width, size_t height)
    : _width(width)
    , _height(height)
    , _data(RT_IMG_CHANNELS * width * height) 
  {}

  Image(T *idata, size_t w, size_t h) : Image(w, h) {
    // TODO: 4-channel, n-channel input images
    for (size_t x=0; x<_width; x++) {
      for (size_t y=0; y<_height; y++) {
        size_t px = index(x, y);
        _data[px + 0] = idata[px + 0];
        _data[px + 1] = idata[px + 1];
        _data[px + 2] = idata[px + 2];
      }
    }
  }

  void set(size_t x, size_t y, const Vec3 &v) {
    size_t px = index(x, y);
    _data[px + 0] = v.x();
    _data[px + 1] = v.y();
    _data[px + 2] = v.z();
  }

  void set(size_t x, size_t y, T r, T g, T b) {
    size_t px = index(x, y);
    _data[px + 0] = r;
    _data[px + 1] = g;
    _data[px + 2] = b;
  }

  void get(size_t x, size_t y, Vec3 &v) const {
    v[0] = get_channel(x, y, 0);
    v[1] = get_channel(x, y, 1);
    v[2] = get_channel(x, y, 2);
  }

  inline size_t index(size_t x, size_t y) const {
    assert(x<_width);
    assert(y<_height);
    return  x * RT_IMG_CHANNELS + y * _width * RT_IMG_CHANNELS;
  }

  inline T get_channel(size_t x, size_t y, size_t c) const {
    return _data[index(x, y) + c];
  }

  size_t _width;
  size_t _height;
  std::vector<T> _data;
};

#endif