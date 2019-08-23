#include <iostream>
#include <vector>
#include <memory>
#include <random>
#include <chrono>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

#include "raytracer/Camera.h"
#include "raytracer/HitRecord.h"
#include "raytracer/Image.h"
#include "raytracer/integrators/BasicIntegrator.h"
#include "raytracer/materials/materials.h"
#include "raytracer/Scene.h"
#include "raytracer/shapes/BvhNode.h"
#include "raytracer/shapes/ConstantMedium.h"
#include "raytracer/shapes/Environment.h"
#include "raytracer/shapes/Shape.h"
#include "raytracer/shapes/InvertedObject.h"
#include "raytracer/shapes/ShapeInstance.h"
#include "raytracer/shapes/Plane.h"
#include "raytracer/shapes/MeshCube.h"
#include "raytracer/shapes/MeshRectangle.h"
#include "raytracer/shapes/MeshSphere.h"
#include "raytracer/shapes/RectXZ.h"
#include "raytracer/shapes/Sphere.h"
#include "raytracer/random_numbers.h"
#include "raytracer/Timer.h"
#include "raytracer/Perlin.h"
#include "raytracer/Ray.h"
#include "raytracer/Vec3.h"

#define PI 3.14159f

using namespace raytracer;

template<typename T>
T lerp(const T&a, const T&b, float t) {
  return (1.0f - t) * a + t * b;
}

Vec3 replace_nan(const Vec3 &a) {
  if (a.x() != a.x()
    || a.y() != a.y()
    || a.z() != a.z())
    return Vec3::Zero();
  return a;
}

/* 
TODO

x create bounds for plane
- proper fresnel
- sobel lines
- asm pbr
- other primitives?
  x plane
  x sphere
  - mesh
  - rectangle
  - triangle
- fractals
x tbb parallelization
x eigen!
x finish week of raytracer
  x textures
  x perlin noise
  x image textures
  x instances / transformation
  x volume material
  x cornell box
- finish raytracer rest of life
  - convert to monte carlo
  - convert materials
    - lambert
    - dielectric
    - metal
  - create light shape
    - define pdfs for hitables
*/

struct Config {
  size_t image_width = 300;
  size_t image_height = 300;
  size_t max_bounces = 8;
  size_t samples_per_pixel = 10;
} config;

Vec3 normal_to_color(const Vec3 &n) {
  return 0.5f * (n + Vec3(1.0f, 1.0f, 1.0f));
}

std::shared_ptr<Camera> create_default_camera(size_t _width, size_t _height) {

  Eigen::Affine3f camera_tx = Eigen::Translation3f( 0, 1.5f, 2.75 ) * Eigen::Affine3f::Identity();

  return std::shared_ptr<Camera>(new Camera(camera_tx,
    70.0f, // vfov
    float(_width)/float(_height), // aspect
    0.0f, // aperture
    2.75f // focus dist
  ));
}

std::shared_ptr<Scene> create_single_sphere_scene() {
  auto scene = std::make_shared<Scene>();

  auto noise_mat = std::make_shared<Lambertian>(
    std::make_shared<NoiseTexture>()
  );

  auto light_mat = std::make_shared<DiffuseLight>(
    std::make_shared<ConstantTexture>(5.0f)
  );

  std::shared_ptr<ImageTexture> earth_texture;
  {
    int w, h, num_channels;
    uint8_t *img = stbi_load("./resources/textures/earth_day.jpg", &w, &h, &num_channels, RT_IMG_CHANNELS);
    earth_texture = std::make_shared<ImageTexture>(
      Image<uint8_t>(img, w, h)
    );
  }

  auto earth_mat = std::make_shared<Lambertian>(
    earth_texture
  );

  scene->create_object<Plane>(Vec3(0.0f, 0.0f, 0.0f),
    Vec3(0.0f, 1.0f, 0.0f),
    light_mat
  );

  /*
  scene->add_object(std::make_shared<Rectangle>(Vec3(2.0f, 1.0f, -1.0f),
    Vec3(2.0f, 1.25f, 0.0f),
    noise_mat
  ));*/

  float sphere_radius = 1.5f;
  scene->create_object<Sphere>(
    Vec3(0.0f, sphere_radius, -1.0f), 
    sphere_radius,
    earth_mat
  );

  return scene;
}

std::shared_ptr<Scene> create_nine_sphere_scene() {
  auto scene = std::make_shared<Scene>();

  scene->create_object<Environment>(std::make_shared<GradientTexture>(
    Vec3(1, 1, 1),
    0.5f * Vec3(0.5f, 0.7f, 1.0f)
  ));

  // ground plane
  auto checker = std::make_shared<CheckerTexture>(
    std::make_shared<ConstantTexture>(Vec3(0, 0, 0)),
    std::make_shared<ConstantTexture>(Vec3(1, 1, 1))
  );
  auto ground_mat = std::make_shared<Lambertian>( checker); 
  scene->create_object<Plane>(Vec3(0.0f, 0.0f, 0.0f),
    Vec3(0.0f, 1.0f, 0.0f),
    ground_mat
  );

  // hero spheres
  int variations = 3;
  float sphere_radius = 0.5f;
  for (int i = 0; i < variations; i++) {
    float t = float(i)/(variations-1);
    float y = 1.33f * sphere_radius * variations * t + sphere_radius;

    auto mirror = create_mirror_material();
    mirror->_roughness = std::make_shared<ConstantTexture>(t);
    scene->add_object(std::make_shared<Sphere>(Vec3(0.0, y, 0.0f),
      sphere_radius,
      mirror
    ));

    auto lambert = create_lambert_material();
    scene->add_object(std::make_shared<Sphere>(Vec3(1.0, y, 0.0f),
      sphere_radius,
      lambert
    ));

    auto lens = create_lens_material();
    lens->_roughness = std::make_shared<ConstantTexture>(float(i)/(variations-1));
    scene->add_object(std::make_shared<Sphere>(
      Vec3(-1.0f, y, 0.0f), 
      sphere_radius,
      lens
    ));
  }

  // litter spheres
  {
    int num_spheres = 100;
    float distance = 5.0f;
    float litter_radius = 0.08f;
    float max_velocity = 0.3f;
    for (int i = 0; i < num_spheres; i++) {
      Vec3 center(0, litter_radius, -distance/2.0f);

      Vec3 offset(2.0f * distance * (-0.5f + random_number()),
        0.0f,
        2.0f * distance * (-0.5f + random_number()));
      
      float P_bouncing = 0.5f;
      float velocity_y = random_number() < P_bouncing? max_velocity * random_number() : 0.0f;

      auto litter_mat = std::make_shared<Lambertian>(Vec3(random_number(), random_number(), random_number()));
      auto sphere = std::make_shared<Sphere>(
        Vec3(0, 0, 0),
        litter_radius,
        litter_mat,
        Vec3(0.0, velocity_y, 0.0)
      );

      Eigen::Affine3f transform = Eigen::Translation3f(center + offset) * Eigen::Affine3f::Identity();

      auto instance = std::make_shared<ShapeInstance>(
        sphere,
        transform
      );

      scene->add_object(instance);
    }
  }

  return scene;
}


std::shared_ptr<Scene> create_cornell_box() {
  float room_size = 10.0f;
  float room_half = room_size/2.0f;

  auto scene = std::make_shared<Scene>();

  Eigen::Affine3f camera_tx = Eigen::Translation3f(0, room_size/2.0f, 10)
    * Eigen::Affine3f::Identity();

  scene->_camera = std::shared_ptr<Camera>(new Camera(
    camera_tx,
    70.0f, // vfov
    1.0f, // aspect
    0.0f, // aperture
    2.75f // focus dist
  ));

  auto lambert_gray = std::make_shared<Lambertian>(Vec3(0.73f, 0.73f, 0.73f));
  auto lambert_red = std::make_shared<Lambertian>(Vec3(0.65f, 0.05f, 0.05f));
  auto lambert_green = std::make_shared<Lambertian>(Vec3(0.12f, 0.45f, 0.15f));

  float rect_size = room_size;
  float rect_half = rect_size/2.0f;

  // light
  {
    float light_size = 2.5f;
    float light_half = light_size/2.0f;
    float light_distance = 0.0f; // room_half/2.0f - light_size;
    float light_height = room_size - 0.001f;

    auto light_mat = std::make_shared<DiffuseLight>(std::make_shared<ConstantTexture>(15.0f));

    Eigen::Affine3f light_tx = 
      Eigen::Translation3f(0, light_height, -light_distance) *
      Eigen::Scaling(light_size) *
      Eigen::Affine3f::Identity();

    auto rect_mesh = std::make_shared<MeshRectangle>(
      10, 10,
      light_mat
    );
    
    std::shared_ptr<Group> group = std::make_shared<Group>();
    group->add_object(std::make_shared<ShapeInstance>(rect_mesh, light_tx, light_mat));
    scene->add_object(group);
  }

  auto sphere_instance = std::make_shared<Sphere>(Vec3(0, 1.0f, 0), 1.0f, nullptr);
  auto foggy_sphere = std::make_shared<ConstantMedium>(
    sphere_instance,
    0.8f, 
    std::make_shared<ConstantTexture>(Vec3(1.0f, 1.0f, 1.0f)));
  scene->add_object(foggy_sphere);

  // red sphere
  {
    auto sphere_mesh = std::make_shared<Sphere>(Vec3(0, 0, 0), 1.0f, lambert_red);
    Eigen::Affine3f sphere_tx = Eigen::Translation3f(room_half/4.0f, 1.0f, -2.4f) * Eigen::Affine3f::Identity();
    scene->create_object<ShapeInstance>(sphere_mesh, sphere_tx);
  }

  // metal sphere
  {
    float sphere_size = 2.0f;
    auto sphere_mesh = std::make_shared<Sphere>(Vec3(0, 0, 0), sphere_size, create_mirror_material());
    Eigen::Affine3f sphere_tx = Eigen::Translation3f(-room_half/2.0f, sphere_size, -2.0f) * Eigen::Affine3f::Identity();
    scene->create_object<ShapeInstance>(sphere_mesh, sphere_tx);
  }

  // glass sphere
  {
    float sphere_size = 1.0f;
    auto glass_mat = create_lens_material();
    glass_mat->_ior = 1.5f;

    auto sphere_mesh = std::make_shared<Sphere>(Vec3(0, 0, 0), sphere_size, glass_mat);
    Eigen::Affine3f sphere_tx = Eigen::Translation3f(room_half/2.0f, sphere_size + 2.0f, 0) * Eigen::Affine3f::Identity();
    scene->create_object<ShapeInstance>(sphere_mesh, sphere_tx);
  }

  // cube
  { 
    auto cube = std::make_shared<MeshCube>(lambert_red);
    float cube_size = 2.0f;
    Eigen::Affine3f cube_tx1 =
      Eigen::Translation3f(Vec3(room_half/2, cube_size/2.0f, 0)) *
      Eigen::AngleAxisf(0.15f * 2.0f * PI, Vec3::UnitY()) *
      Eigen::Scaling(cube_size) *
      Eigen::Affine3f::Identity();
    scene->create_object<ShapeInstance>(cube,  cube_tx1, lambert_gray);

    Eigen::Affine3f cube_tx2 =
      Eigen::Translation3f(Vec3(-room_half/3, cube_size/4.0f, 1.0f)) *
      Eigen::AngleAxisf(-0.15f * 2.0f * PI, Vec3::UnitY()) *
      Eigen::Scaling(cube_size/2.0f) *
      Eigen::Affine3f::Identity();

    // auto cube_volume = std::make_shared<ShapeInstance>(cube,  cube_tx2);
    scene->create_object<ShapeInstance>(cube, 
      cube_tx2,
      lambert_gray);
  }

  // walls
  auto wall = std::make_shared<MeshRectangle>(
    10, 10,
    lambert_gray
  );
  Eigen::Affine3f tx_scale = Eigen::Scaling(room_size) * Eigen::Affine3f::Identity();

  // ground
  scene->create_object<ShapeInstance>(wall, tx_scale);

  if (1)
  {

    // side wall
    scene->create_object<ShapeInstance>(wall,
      Eigen::Translation3f(Vec3(-room_half, room_half, 0)) * Eigen::AngleAxisf(-PI / 2, Vec3::UnitZ()) * tx_scale,
      lambert_red);

    // side wall
    scene->create_object<ShapeInstance>(wall,
      Eigen::Translation3f(Vec3(room_half, room_half, 0)) * Eigen::AngleAxisf(PI / 2, Vec3::UnitZ()) * tx_scale,
      lambert_green);

    // ceiling
    scene->create_object<ShapeInstance>(wall,
      Eigen::Translation3f(Vec3(0, room_size, 0)) * Eigen::AngleAxisf(-PI, Vec3::UnitZ()) * tx_scale);

    // rear wall
    scene->create_object<ShapeInstance>(wall,
      Eigen::Translation3f(Vec3(0, room_half, -room_half)) * Eigen::AngleAxisf(PI/2, Vec3::UnitX()) * tx_scale);
  }

  return scene;
}

int main(int argc, char **argv) {
  // commas in numeric output
  std::cout.imbue(std::locale(""));

  std::shared_ptr<Image<float>> img = std::make_shared<Image<float>>(config.image_width, config.image_height);
  
  auto scene = create_cornell_box();
  if (!scene->_camera) scene->_camera = create_default_camera(img->_width, img->_height);

  float t_begin = 0.0f;
  float t_end = 0.0f;
 
  {
    Timer render = Timer("render time");

    BasicIntegrator integrator(img, 
      config.samples_per_pixel, 
      config.max_bounces,
      t_begin, t_end);
    integrator.render(*scene);

    std::cout << "ray count: " << Ray::_ray_count << " rays" << std::endl;
  }

  // convert to output format
  Image<uint8_t> img_out(img->_width, img->_height);
  {
    for(size_t x = 0; x < img->_width; x++) {
      Vec3 v_out;
      for (size_t y = 0; y < img->_height; y++) {
        img->get(x, y, v_out);

        // tonemap
        v_out[0] = std::min(1.0f, sqrtf(v_out.x()));
        v_out[1] = std::min(1.0f, sqrtf(v_out.y()));
        v_out[2] = std::min(1.0f, sqrtf(v_out.z()));

        // remap from 0.0-1.0 to 0-255
        v_out *= 255.99f;
        img_out.set(
          x, y,
          (uint8_t)v_out.x(), 
          (uint8_t)v_out.y(), 
          (uint8_t)v_out.z()
        );
      }
    }
  }

  stbi_write_png("./out.png", 
    (int)img_out._width, 
    (int)img_out._height, 
    RT_IMG_CHANNELS, 
    img_out._data.data(),
    (int)img_out._width * RT_IMG_CHANNELS);

  std::cout << "OK" << std::endl;

  return 0;
}
