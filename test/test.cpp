#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"

#include "raytracer/AxisAlignedBoundingBox.h"
#include "raytracer/Image.h"
#include "raytracer/shapes/BvhNode.h"
#include "raytracer/shapes/Sphere.h"
#include "raytracer/textures.h"
#include "raytracer/Vec3.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

using namespace raytracer;

TEST_CASE("a test", "[smoke]") {
  REQUIRE(true);
}

TEST_CASE("vec3", "[vec3]") {
  Vec3 zero_one(0);
  Vec3 zero_three(0, 0, 0);

  REQUIRE(zero_one[0] == 0);
  REQUIRE(zero_one[1] == 0);
  REQUIRE(zero_one[2] == 0);

  REQUIRE(zero_three[1] == 0);
  REQUIRE(zero_three[2] == 0);
  REQUIRE(zero_three[3] == 0);
  REQUIRE(zero_one == zero_three);

  Vec3 one(1, 1, 1);
  REQUIRE(one == one);

  REQUIRE(one[0] == 1.0f);
  REQUIRE(one[1] == 1.0f);
  REQUIRE(one[2] == 1.0f);

  REQUIRE((Vec3(1, 1, 1) - Vec3(1, 1, 1)) == Vec3(0, 0, 0));
  REQUIRE((Vec3(0, 0, 0) + Vec3(1, 1, 1)) == Vec3(1, 1, 1));
  REQUIRE((Vec3::Ones() * 2.0f) == Vec3(2.0f, 2.0f, 2.0f));
}

TEST_CASE("bounding box", "[aabb]") {
  AxisAlignedBoundingBox aabb(Vec3(0, 0, 0), Vec3(1, 1, 1));

  aabb.extend({Vec3(-1, -1, -1), Vec3(2, 2, 2)});

  std::cout << aabb.min() << std::endl;

  REQUIRE((aabb.min() == Vec3(-1, -1, -1)));
  REQUIRE((aabb.max() == Vec3(2, 2, 2)));
}

TEST_CASE("sphere bounding box", "[aabb][sphere]") {
  AxisAlignedBoundingBox box;

  SECTION("simple")
  {
    Sphere sphere(Vec3(0, 0, 0), 1.0f, nullptr, Vec3(0, 0, 0));
    bool result = sphere.bounding_box(0.0f, 0.0f, box);
    REQUIRE(result);
    REQUIRE(box.min() == Vec3(-1.0f, -1.0f, -1.0f));
    REQUIRE(box.max() == Vec3(1, 1, 1));
  }

  SECTION("differing origin, moving")
  {
    Sphere sphere(Vec3(1, 0, -1), 1.0f, nullptr, Vec3(1.0f, 0, 0));
    bool result = sphere.bounding_box(0, 0, box);
    REQUIRE(result);
    REQUIRE(box.min() == Vec3(0, -1, -2));
    REQUIRE(box.max() == Vec3(2, 1, 0));

    result = sphere.bounding_box(0, 1.0f, box);
    REQUIRE(result);
    REQUIRE(box.min() == Vec3(0, -1, -2));
    REQUIRE(box.max() == Vec3(3, 1, 0));
  }
}

TEST_CASE("bvh", "[bvh]") {
  std::vector<std::shared_ptr<Shape> > scene = {
    std::make_shared<Sphere>(Vec3(0.f, 0.f, 0.f), 1.0f, nullptr, Vec3(0.f, 0.f, 0.f)),
    std::make_shared<Sphere>(Vec3(1.f, 1.f, 1.f), 1.0f, nullptr, Vec3(0.f, 0.f, 0.f)),
    std::make_shared<Sphere>(Vec3(2.f, 2.f, 2.f), 1.0f, nullptr, Vec3(0.f, 0.f, 0.f)),
    std::make_shared<Sphere>(Vec3(3.f, 3.f, 3.f), 1.0f, nullptr, Vec3(0.f, 1.f, 0.f)),
  };

  auto eval_box = [&scene](float t0, float t1) -> bool {
    BvhNode bvh(scene.begin(), scene.end(), t0, t1);
    REQUIRE(bvh.count() == scene.size());
  
    AxisAlignedBoundingBox box;
    bool result = bvh.bounding_box(t0, t1, box);

    REQUIRE(result);

    Vec3 lerped = (1.0f - (t1 - t0)) * Vec3(4, 4, 4) + (t1 - t0) * Vec3(4, 5, 4);
    REQUIRE(box.min() == Vec3(-1, -1, -1));
    REQUIRE(box.max() == lerped);
    return true;
  };

  REQUIRE(eval_box(0, 0));
  REQUIRE(eval_box(0, 1));
}

TEST_CASE("bvh hit tests", "[bvh][ray]") {
  std::vector<std::shared_ptr<Shape> > scene = {
    std::make_shared<Sphere>(Vec3(0.f, 0.f, 0.f), 1.f, nullptr, Vec3(0.f, 0.f, 0.f)),
  };

  BvhNode bvh(scene.begin(), scene.end(), 0, 0);

  Ray r(Vec3(0, -3.0f, 0), Vec3(0, 1, 0), 0);
  HitRecord hit;
  bool hit_sphere = scene[0]->hit(r, 0, std::numeric_limits<float>::max(), hit);
  REQUIRE(hit_sphere);

  bool hit_bvh = bvh.hit(r, 0, std::numeric_limits<float>::max(), hit);
  REQUIRE(bvh.count() >= 1);
  REQUIRE(hit_bvh);
}

TEST_CASE("images", "[img]") {
  Image<float> img(32, 32);

  REQUIRE(img._width == 32);
  REQUIRE(img._height == 32);

  for (size_t x=0; x<img._width; x++) {
    for (size_t y=0; y<img._height; y++) {
      img.set(x, y, {
        x/(float)img._width,
        y/(float)img._height,
        0.0f
      });
    }
  }

  Vec3 color;
  for (size_t x=0; x<img._width; x++) {
    for (size_t y=0; y<img._height; y++) {
      img.get(x, y, color);

      REQUIRE(color.x() == x/(float)img._width);
      REQUIRE(color.y() == y/(float)img._height);
      REQUIRE(color.z() == 0);
    }
  }

  size_t w = 4096;
  size_t h = 8192;
  std::vector<uint8_t> img_data(w * h * 3);
  Image<uint8_t> img_8(img_data.data(), w, h);

  REQUIRE(img_8._width == w);
  REQUIRE(img_8._height == h);
}

TEST_CASE("image texture", "[img][texture]") {
  size_t w = 4096;
  size_t h = 8192;
  std::vector<uint8_t> img_data(w * h * 3);
  std::shared_ptr<ImageTexture> texture = std::make_shared<ImageTexture>(
    Image<uint8_t>(img_data.data(), w, h)
  );

  REQUIRE(texture->_image._width == w);
  REQUIRE(texture->_image._height == h);

  int iw, ih, num_channels;
  uint8_t *stb_data = stbi_load("./resources/textures/earth_day.jpg", &iw, &ih, &num_channels, RT_IMG_CHANNELS);
  ImageTexture stb_texture(
    Image<uint8_t>(stb_data, iw, ih)
  );
  REQUIRE(stb_texture._image._width == iw);
  REQUIRE(stb_texture._image._height == ih);
}


