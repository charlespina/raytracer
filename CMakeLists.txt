cmake_minimum_required(VERSION 3.13)

project (Raytracer)

set (CMAKE_CXX_STANDARD 14)

include_directories (
    external/stb/include
    src/include
)

# targets
add_executable (Raytracer 

    src/Camera.cpp
    src/materials.cpp
    src/random_numbers.cpp
    src/Plane.cpp
    src/Sphere.cpp
    src/main.cpp)
