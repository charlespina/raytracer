#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"

#include "raytracer/AxisAlignedBoundingBox.h"
#include "raytracer/BvhNode.h"
#include "raytracer/Sphere.h"
#include "raytracer/Vec3.h"

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

  Vec3 one(1.0f);
  REQUIRE(one == one);

  REQUIRE(one[0] == 1.0f);
  REQUIRE(one[1] == 1.0f);
  REQUIRE(one[2] == 1.0f);

  REQUIRE((Vec3(1) - Vec3(1)) == Vec3(0));
  REQUIRE((Vec3(0) + Vec3(1)) == Vec3(1));
  REQUIRE((Vec3(1) * Vec3(1)) == Vec3(1));
  REQUIRE((Vec3(1) * 2.0f) == Vec3(2.0f));
}

TEST_CASE("bounding box", "[aabb]") {
  AxisAlignedBoundingBox aabb(Vec3(0), Vec3(1));

  aabb.combine({Vec3(-1), Vec3(2)});

  std::cout << aabb.min() << std::endl;

  REQUIRE((aabb.min() == Vec3(-1)));
  REQUIRE((aabb.max() == Vec3(2)));
}

TEST_CASE("sphere bounding box", "[aabb][sphere]") {
  AxisAlignedBoundingBox box;

  SECTION("simple")
  {
    Sphere sphere(Vec3(0), 1.0f, nullptr, Vec3(0));
    bool result = sphere.bounding_box(0.0f, 0.0f, box);
    REQUIRE(result);
    REQUIRE(box.min() == Vec3(-1.0f));
    REQUIRE(box.max() == Vec3(1.0f));
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
  std::vector<std::shared_ptr<IHitable> > scene = {
    std::make_shared<Sphere>(Vec3(0), 1, nullptr, Vec3(0)),
    std::make_shared<Sphere>(Vec3(1), 1, nullptr, Vec3(0)),
    std::make_shared<Sphere>(Vec3(2), 1, nullptr, Vec3(0)),
    std::make_shared<Sphere>(Vec3(3), 1, nullptr, Vec3(0, 1, 0)),
  };

  auto eval_box = [&scene](float t0, float t1) -> bool {
    BvhNode bvh(scene.begin(), scene.end(), t0, t1);
    REQUIRE(bvh.count() == scene.size());
  
    AxisAlignedBoundingBox box;
    bool result = bvh.bounding_box(t0, t1, box);

    REQUIRE(result);

    Vec3 lerped = (1.0f - (t1 - t0)) * Vec3(4) + (t1 - t0) * Vec3(4, 5, 4);
    REQUIRE(box.min() == Vec3(-1));
    REQUIRE(box.max() == lerped);
    return true;
  };

  REQUIRE(eval_box(0, 0));
  REQUIRE(eval_box(0, 1));
}

TEST_CASE("bvh hit tests", "[bvh][ray]") {
  std::vector<std::shared_ptr<IHitable> > scene = {
    std::make_shared<Sphere>(Vec3(0), 1, nullptr, Vec3(0)),
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




