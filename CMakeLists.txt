cmake_minimum_required(VERSION 3.15)

project (Raytracer)

include_directories (
    external/stb/include
    src/include
)

# targets
add_executable (Raytracer src/main.cpp)