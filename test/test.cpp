#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"

#include "raytracer/AxisAlignedBoundingBox.h"
#include "raytracer/Sphere.h"
#include "raytracer/Vec3.h"

TEST_CASE("a test", "[smoke]") {
    REQUIRE(true);
}

TEST_CASE("bounding box", "[aabb]") {
    AxisAlignedBoundingBox aabb(Vec3(0), Vec3(1));

    aabb.combine({Vec3(-1), Vec3(2)});

    std::cout << aabb.min() << std::endl;

    REQUIRE((aabb.min() == Vec3(-1)));
    REQUIRE((aabb.max() == Vec3(2)));
}

TEST_CASE("sphere bounding box", "[aabb][sphere]") {
    Sphere sphere(Vec3(0), 1.0f, nullptr, Vec3(0));
    AxisAlignedBoundingBox box;
    bool result = sphere.bounding_box(0.0f, 0.0f, box);
    REQUIRE(result);
    REQUIRE(box.min() == Vec3(-1.0f));
    REQUIRE(box.max() == Vec3(1.0f));
}




