#include <iostream>
#include <vector>
#include <memory>
#include <random>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

#include "raytracer/Camera.h"
#include "raytracer/HitRecord.h"
#include "raytracer/IHitable.h"
#include "raytracer/Image.h"
#include "raytracer/materials.h"
#include "raytracer/random_numbers.h"
#include "raytracer/Scene.h"
#include "raytracer/Sphere.h"
#include "raytracer/ray.h"
#include "raytracer/vec3.h"

#define RT_IMG_WIDTH 400
#define RT_IMG_HEIGHT 200
#define RT_MAX_BOUNCES 50
#define RT_NUM_SAMPLES 10

vec3 normal_to_color(const vec3 &n) {
  return 0.5f * (n + vec3(1.0f, 1.0f, 1.0f));
}

vec3 raycast(const ray &r, Scene &scn, size_t depth) {
  HitRecord record;
  if (scn.hit(r, 0.001f, std::numeric_limits<float>::max(), record)) {
    ray scattered;
    vec3 attenuation;
    if (depth < RT_MAX_BOUNCES && record.material->scatter(r, record, attenuation, scattered)) {
      return attenuation * raycast(scattered, scn, depth+1);
    } else {
      return vec3(0, 0, 0);
    }
    // normal_to_color(record.normal);
  } else {
    vec3 unit_direction = unit_vector(r.direction());
    float t = 0.5f * (unit_direction.y() + 1.0f);
    return (1.0f - t) * vec3(1.0f, 1.0f, 1.0f) + t * vec3(0.5f, 0.7f, 1.0f);
  }
}

std::shared_ptr<Scene> create_scene() {
  auto scene = std::make_shared<Scene>();

  float sphere_radius = 0.5f;

  // ground "plane"
  scene->_geometries.push_back(std::make_shared<Sphere>(vec3(0.0f, -100.5f, -1.0),
    100.0,
    std::make_shared<Lambertian>(vec3(0.8, 0.8, 0.0))
  ));

  scene->_geometries.push_back(std::make_shared<Sphere>(vec3(0.0, 0.0f, -1.0f),
    sphere_radius,
    std::make_shared<Metal>(vec3(0.8, 0.6, 0.2), 1.0f)
  ));
  scene->_geometries.push_back(std::make_shared<Sphere>(vec3(1.0, 0.0f, -1.0f),
    sphere_radius,
    std::make_shared<Dielectric>(0.3f, 1.5f)
  ));
  scene->_geometries.push_back(std::make_shared<Sphere>(
    vec3(-1.0f, 0.0f, -1.0f), 
    sphere_radius,
    std::make_shared<Lambertian>(vec3(0.8, 0.3, 0.3))
  ));

  return scene;
}

int main(int argc, char **argv) {
    Image<float> img(RT_IMG_WIDTH, RT_IMG_HEIGHT);
    
    Camera camera({ 0, 0, 0 },
      {0, 0, -1},
      {0, 1, 0},
      90.0f,
      float(img._width)/float(img._height));
    
    auto scene = create_scene();
  
    for (size_t x = 0; x < img._width; x++) {
      for (size_t y = 0; y < img._height; y++) {
        vec3 color(0, 0, 0);
        
        for (size_t sample=0; sample < RT_NUM_SAMPLES; sample++) {
          float u = float(x + random_number()) / float(img._width);
          float v = float(y + random_number()) / float(img._height);
        
          ray r = camera.get_ray(u, v);
          color += raycast(r, *scene.get(), 0);
        }
        
        color /= float(RT_NUM_SAMPLES);

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
    
    std::cout << "OK\n";

    return 0;
}
