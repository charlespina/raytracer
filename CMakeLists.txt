cmake_minimum_required(VERSION 3.13)

project (raytracer)
set (PROJECT_LIB ${PROJECT_NAME}_lib)
set (MAIN_EXE ${PROJECT_NAME})
set (TEST_EXE test_${PROJECT_NAME})

set (CMAKE_CXX_STANDARD 14)

include_directories (
    external/stb/include
    include
)

set (SOURCES
    src/AxisAlignedBoundingBox.cpp
    src/BvhNode.cpp
    src/Camera.cpp
    src/materials.cpp
    src/random_numbers.cpp
    src/Plane.cpp
    src/Sphere.cpp)

add_library(${PROJECT_LIB} STATIC ${SOURCES})

# main executable
add_executable (${MAIN_EXE} src/main.cpp)
target_link_libraries(${MAIN_EXE} ${PROJECT_LIB})

# test executable
add_executable (${TEST_EXE} test/test.cpp)
target_link_libraries(${TEST_EXE} ${PROJECT_LIB})
target_include_directories(${TEST_EXE} PUBLIC
    external/catch2/include
)