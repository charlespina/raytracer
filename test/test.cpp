#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"

#include "raytracer/AxisAlignedBoundingBox.h"
#include "raytracer/Vec3.h"

TEST_CASE("a test", "[smoke]") {
    REQUIRE(true);
}

TEST_CASE("bounding box", "[aabb]") {
    AxisAlignedBoundingBox aabb(Vec3(0), Vec3(1));
}