#include <iostream>
#include <vector>
#include <memory>
#include <random>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

#include "raytracer/BvhNode.h"
#include "raytracer/Camera.h"
#include "raytracer/HitRecord.h"
#include "raytracer/IHitable.h"
#include "raytracer/Image.h"
#include "raytracer/materials.h"
#include "raytracer/Plane.h"
#include "raytracer/random_numbers.h"
#include "raytracer/Scene.h"
#include "raytracer/Sphere.h"
#include "raytracer/ray.h"
#include "raytracer/vec3.h"


struct Config {
  size_t image_width = 300;
  size_t image_height = 300;
  size_t max_bounces = 50;
  size_t samples_per_pixel = 10;
} config;

vec3 normal_to_color(const vec3 &n) {
  return 0.5f * (n + vec3(1.0f, 1.0f, 1.0f));
}

vec3 raycast(const ray &r, IHitable &hitable, size_t depth) {
  HitRecord record;
  if (hitable.hit(r, 0.001f, std::numeric_limits<float>::max(), record)) {
    ray scattered;
    vec3 attenuation;
    if (depth < config.max_bounces && record.material->scatter(r, record, attenuation, scattered)) {
      return attenuation * raycast(scattered, hitable, depth+1);
    } else {
      return vec3(0, 0, 0);
    }
  } else {
    vec3 unit_direction = unit_vector(r.direction());
    float t = 0.5f * (unit_direction.y() + 1.0f);
    return (1.0f - t) * vec3(1.0f, 1.0f, 1.0f) + t * vec3(0.5f, 0.7f, 1.0f);
  }
}

std::shared_ptr<BvhNode> build_bvh(std::vector<std::shared_ptr<IHitable> > geoms, float t0, float t1) {
  auto copied_geom_list = geoms;
  return std::make_shared<BvhNode>(copied_geom_list.begin(), copied_geom_list.end(), t0, t1);
}

std::shared_ptr<Scene> create_scene() {
  auto scene = std::make_shared<Scene>();

  // ground "plane"
  scene->_geometries.push_back(std::make_shared<Plane>(vec3(0.0f, 0.0f, 0.0f),
    vec3(0.0f, 1.0f, 0.0f),
    std::make_shared<Lambertian>(vec3(0.2, 0.6, 0.4))
  ));

  // hero spheres
  int variations = 3;
  float sphere_radius = 0.5f;
  for (int i = 0; i < variations; i++) {
    float t = float(i)/(variations-1);
    float y = 1.33f * sphere_radius * variations * t + sphere_radius;

    auto mirror = create_mirror_material();
    mirror->_roughness = t;
    scene->_geometries.push_back(std::make_shared<Sphere>(vec3(0.0, y, 0.0f),
      sphere_radius,
      mirror
    ));

    auto lambert = create_lambert_material();
    scene->_geometries.push_back(std::make_shared<Sphere>(vec3(1.0, y, 0.0f),
      sphere_radius,
      lambert
    ));

    auto lens = create_lens_material();
    lens->_roughness = float(i)/(variations-1);
    scene->_geometries.push_back(std::make_shared<Sphere>(
      vec3(-1.0f, y, 0.0f), 
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
      vec3 center(0, litter_radius, -distance/2.0f);

      vec3 offset(2.0f * distance * (-0.5 + random_number()),
        0.0f,
        2.0f * distance * (-0.5f + random_number()));
      
      float P_bouncing = 0.5f;
      float velocity_y = random_number() < P_bouncing? max_velocity * random_number() : 0.0f;

      auto litter_mat = std::make_shared<Lambertian>(vec3(random_number(), random_number(), random_number()));
      auto sphere = std::make_shared<Sphere>(
        center + offset,
        litter_radius,
        litter_mat,
        vec3(0.0, velocity_y, 0.0)
      );
      scene->_geometries.push_back(sphere);
    }
  }

  return scene;
}

int main(int argc, char **argv) {
    Image<float> img(config.image_width, config.image_height);
    
    Camera camera({ 0, 1.5f, 2.75 }, // pos
      {0, 1.5f, 0}, // target
      {0, 1, 0}, // up
      70.0f, // vfov
      float(img._width)/float(img._height), // aspect
      0.3f, // aperture
      2.75f // focus dist
     );
    
    auto scene = create_scene();
    float t_begin = 0.0f;
    float t_end = 1.0f;
    auto bvh = scene; // build_bvh(scene->_geometries, t_begin, t_end);
  
    for (size_t x = 0; x < img._width; x++) {
      for (size_t y = 0; y < img._height; y++) {
        vec3 color(0, 0, 0);
        
        for (size_t sample=0; sample < config.samples_per_pixel; sample++) {
          float u = float(x + random_number()) / float(img._width);
          float v = float(y + random_number()) / float(img._height);
        
          ray r = camera.get_ray(u, v, t_begin, t_end);
          color += raycast(r, *bvh.get(), 0);
        }
        
        color /= float(config.samples_per_pixel);

        // a little y-flip
        img.set(x, img._height - y - 1, color);
      }
    }

    // convert to output format
    Image<uint8_t> img_out(img._width, img._height);
    vec3 v_out;
    for (size_t x = 0; x < img._width; x++) {
      for (size_t y = 0; y < img._height; y++) {
        img.get(x, y, v_out);

        // tonemap
        v_out.e[0] = sqrtf(v_out.r());
        v_out.e[1] = sqrtf(v_out.g());
        v_out.e[2] = sqrtf(v_out.b());

        // remap from 0.0-1.0 to 0-255
        v_out *= 255.99f;
        img_out.set(
          x, y,
          (uint8_t)v_out.r(), 
          (uint8_t)v_out.g(), 
          (uint8_t)v_out.b()
        );
      }
    }

    stbi_write_png("./out.png", 
      img_out._width, 
      img_out._height, 
      RT_IMG_CHANNELS, 
      img_out._data.data(),
      img_out._width * RT_IMG_CHANNELS);
    
    std::cout << "OK" << std::endl;

    return 0;
}
