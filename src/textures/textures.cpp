#include "raytracer/textures/textures.h"

namespace raytracer {

float channel_8bpc_to_float(uint8_t c) {
  float v = c/255.0f;
  return v*v; // poor man's srgb to linear!
}

ImageTexture::ImageTexture(const Image<float> &img) : _image(img) {
}

ImageTexture::ImageTexture(const Image<uint8_t> &img_8) : _image(img_8._width, img_8._height) {
  for (size_t x=0; x<img_8._width; x++) {
    for (size_t y=0; y<img_8._height; y++) {
      _image.set(x, y, {
        channel_8bpc_to_float(img_8.get_channel(x, y, 0)),
        channel_8bpc_to_float(img_8.get_channel(x, y, 1)),
        channel_8bpc_to_float(img_8.get_channel(x, y, 2))
      });
    }
  }
}

Vec3 ImageTexture::sample_color(float u, float v, const Vec3 &p) const {
  int x = (int)(u * _image._width);
  int y = (int)((1-v) * _image._height - 0.001f); // y-flip
  x = std::max(0, std::min(x, (int)_image._width - 1));
  y = std::max(0, std::min(y, (int)_image._height - 1));
  
  Vec3 c;
  _image.get(x, y, c);
  return c;
}

} // raytracer