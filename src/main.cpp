#include <iostream>
#include <vector>
#include <memory>
#include <random>
#include <chrono>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

#include "raytracer/BvhNode.h"
#include "raytracer/Camera.h"
#include "raytracer/HitRecord.h"
#include "raytracer/IHitable.h"
#include "raytracer/Image.h"
#include "raytracer/Instance.h"
#include "raytracer/materials.h"
#include "raytracer/Plane.h"
#include "raytracer/random_numbers.h"
#include "raytracer/Scene.h"
#include "raytracer/Sphere.h"
#include "raytracer/Perlin.h"
#include "raytracer/Ray.h"
#include "raytracer/Rectangle.h"
#include "raytracer/Vec3.h"

#include "tbb/parallel_for.h"

using Clock = std::chrono::high_resolution_clock;

#define RT_MULTITHREADING_ENABLED 1

void parallel_for(size_t initial_value, size_t count, std::function<void(size_t)> fn) {
  tbb::parallel_for(initial_value, count, fn);
}

void serial_for(size_t initial_value, size_t count, std::function<void(size_t)> fn) {
  for (size_t i=initial_value; i<count; i++) {
    fn(i);
  }
}

#if RT_MULTITHREADING_ENABLED
#define RT_FOR(...) parallel_for(__VA_ARGS__)
#else
#define RT_FOR(...) serial_for(__VA_ARGS__)
#endif

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
- finish week of raytracer
  x textures
  x perlin noise
  x image textures
  x instances / transformation
  - volume material
  - cornell box

*/


struct Config {
  size_t image_width = 300;
  size_t image_height = 300;
  size_t max_bounces = 50;
  size_t samples_per_pixel = 100;
} config;

Vec3 normal_to_color(const Vec3 &n) {
  return 0.5f * (n + Vec3(1.0f, 1.0f, 1.0f));
}

Vec3 raycast(const Ray &r, IHitable &hitable, size_t depth) {
  HitRecord record;
  if (hitable.hit(r, 0.001f, std::numeric_limits<float>::max(), record)) {
    Ray scattered;
    Vec3 accumulated(0, 0, 0);

    Vec3 emitted = record.material->emit(r, record);
    accumulated += emitted;
  
    Vec3 attenuation;
    if (depth < config.max_bounces && record.material->scatter(r, record, attenuation, scattered)) {
      accumulated += (attenuation.array() * raycast(scattered, hitable, depth + 1).array()).matrix();
    }

    return accumulated;
  } else {
    Vec3 unit_direction = unit_vector(r.direction());
    float t = 0.5f * (unit_direction.y() + 1.0f);
    return (1.0f - t) * Vec3(1.0f, 1.0f, 1.0f) + t * Vec3(0.5f, 0.7f, 1.0f);
  }
}

std::shared_ptr<BvhNode> build_bvh(std::vector<std::shared_ptr<IHitable> > geoms, float t0, float t1) {
  auto copied_geom_list = geoms;
  return std::make_shared<BvhNode>(copied_geom_list.begin(), copied_geom_list.end(), t0, t1);
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

  scene->_geometries.push_back(std::make_shared<Plane>(Vec3(0.0f, 0.0f, 0.0f),
    Vec3(0.0f, 1.0f, 0.0f),
    light_mat
  ));

  scene->_geometries.push_back(std::make_shared<Rectangle>(Vec3(2.0f, 1.0f, -1.0f),
    Vec3(2.0f, 1.25f, 0.0f),
    noise_mat
  ));

  float sphere_radius = 1.5f;
  scene->_geometries.push_back(std::make_shared<Sphere>(
    Vec3(0.0f, sphere_radius, -1.0f), 
    sphere_radius,
    earth_mat
  ));

  return scene;
}

std::shared_ptr<Scene> create_nine_sphere_scene() {
  auto scene = std::make_shared<Scene>();

  // ground plane
  auto checker = std::make_shared<CheckerTexture>(
    std::make_shared<ConstantTexture>(Vec3(0, 0, 0)),
    std::make_shared<ConstantTexture>(Vec3(1, 1, 1))
  );
  auto ground_mat = std::make_shared<Lambertian>( checker); 
  scene->_geometries.push_back(std::make_shared<Plane>(Vec3(0.0f, 0.0f, 0.0f),
    Vec3(0.0f, 1.0f, 0.0f),
    ground_mat
  ));

  // hero spheres
  int variations = 3;
  float sphere_radius = 0.5f;
  for (int i = 0; i < variations; i++) {
    float t = float(i)/(variations-1);
    float y = 1.33f * sphere_radius * variations * t + sphere_radius;

    auto mirror = create_mirror_material();
    mirror->_roughness = std::make_shared<ConstantTexture>(t);
    scene->_geometries.push_back(std::make_shared<Sphere>(Vec3(0.0, y, 0.0f),
      sphere_radius,
      mirror
    ));

    auto lambert = create_lambert_material();
    scene->_geometries.push_back(std::make_shared<Sphere>(Vec3(1.0, y, 0.0f),
      sphere_radius,
      lambert
    ));

    auto lens = create_lens_material();
    lens->_roughness = std::make_shared<ConstantTexture>(float(i)/(variations-1));
    scene->_geometries.push_back(std::make_shared<Sphere>(
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

      auto instance = std::make_shared<Instance>(
        sphere,
        transform
      );

      scene->_geometries.push_back(instance);
    }
  }

  return scene;
}

size_t to_ms(Clock::duration duration) {
  return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
}

int main(int argc, char **argv) {
    auto t_start = Clock::now();

    Image<float> img(config.image_width, config.image_height);
    
    Camera camera({ 0, 1.5f, 2.75 }, // pos
      {0, 1.5f, 0}, // target
      {0, 1, 0}, // up
      70.0f, // vfov
      float(img._width)/float(img._height), // aspect
      0.0f, // aperture
      2.75f // focus dist
     );
    
    auto scene = create_nine_sphere_scene();
    float t_begin = 0.0f;
    float t_end = 0.0f;
    auto bvh = build_bvh(scene->_geometries, t_begin, t_end);
  
    RT_FOR((size_t)0, img._width, [=, &img, &camera](size_t x) {
      for (size_t y = 0; y < img._height; y++) {
        Vec3 color(0, 0, 0);
        
        for (size_t sample=0; sample < config.samples_per_pixel; sample++) {
          float u = float(x + random_number()) / float(img._width);
          float v = float(y + random_number()) / float(img._height);
        
          Ray r = camera.get_ray(u, v, t_begin, t_end);
          color += raycast(r, *bvh.get(), 0);
        }
        
        color /= float(config.samples_per_pixel);

        // a little y-flip
        img.set(x, img._height - y - 1, color);
      }
    });

    auto t_render = Clock::now();

    auto t_convert_start = Clock::now();

    // convert to output format
    Image<uint8_t> img_out(img._width, img._height);
    RT_FOR(size_t(0), img._width, [&img, &img_out](size_t x) {
      Vec3 v_out;
      for (size_t y = 0; y < img._height; y++) {
        img.get(x, y, v_out);

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
    });

    auto t_convert_end = Clock::now();

    auto t_write_start = Clock::now();

    stbi_write_png("./out.png", 
      (int)img_out._width, 
      (int)img_out._height, 
      RT_IMG_CHANNELS, 
      img_out._data.data(),
      (int)img_out._width * RT_IMG_CHANNELS);

    auto t_write_end = Clock::now();
    
    std::cout << "render: " << to_ms(t_render - t_start) << " ms" << std::endl;
    std::cout << "convert: " << to_ms(t_convert_end - t_convert_start) << " ms" << std::endl;
    std::cout << "write: " << to_ms(t_write_end - t_write_start) << " ms" << std::endl;
    std::cout << "OK" << std::endl;

    return 0;
}
