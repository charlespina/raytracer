cmake_minimum_required(VERSION 3.13)


project (raytracer)
set (PROJECT_LIB ${PROJECT_NAME}_lib)
set (MAIN_EXE ${PROJECT_NAME})
set (TEST_EXE test_${PROJECT_NAME})

set (CMAKE_CXX_STANDARD 14)

# handle conan dependencies
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)

# configure conan dependencies
#   note: KEEP_RPATHS: tell conan not to set a CMake flag which strips the rpaths
#   from targets
conan_basic_setup(KEEP_RPATHS)

if(APPLE)
    # executables should search for dynamic libraries in their current directory
    set(CMAKE_INSTALL_RPATH "@executable_path")
endif()

# avoid the need to run cmake install to have the rpaths set
set(CMAKE_BUILD_WITH_INSTALL_RPATH ON)

include_directories (
    external/stb/include
    include
)

set (SOURCES
    src/AxisAlignedBoundingBox.cpp
    src/Camera.cpp
    src/materials.cpp
    src/objects/BvhNode.cpp
    src/objects/ConstantMedium.cpp
    src/objects/Group.cpp
    src/objects/Mesh.cpp
    src/objects/MeshRectangle.cpp
    src/objects/MeshSphere.cpp
    src/objects/ObjectInstance.cpp
    src/objects/Plane.cpp
    src/objects/Sphere.cpp
    src/random_numbers.cpp
    src/Perlin.cpp
    src/Ray.cpp
    src/textures.cpp
)

# static library containing our module
add_library(${PROJECT_LIB} STATIC ${SOURCES})

# main executable

add_executable (${MAIN_EXE} src/main.cpp)
target_link_libraries(${MAIN_EXE} ${PROJECT_LIB} ${CONAN_LIBS})

# test executable
add_executable (${TEST_EXE} test/test.cpp)

target_link_libraries(${TEST_EXE} ${PROJECT_LIB} ${CONAN_LIBS})
target_include_directories(${TEST_EXE} PUBLIC
    external/catch2/include
)

# set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
