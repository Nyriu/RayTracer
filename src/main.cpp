//#include <algorithm>
//#include <fstream>
//#include <cmath>
//#include <functional>
#include <iostream>
#include <vector>
#include <limits>
#include <memory> // shared_ptr // todo use
 // https://en.cppreference.com/w/cpp/memory/shared_ptr

#include "Color.h"
#include "geometry.h"
#include "Ray.h"
#include "Camera.h"

#include "ImplicitShape.h"
#include "Light.h"

// SDL
#include <stdlib.h>
#include <SDL2/SDL.h>

#include "scenes.h"

constexpr float kInfinity = std::numeric_limits<float>::max(); 

bool sphereTraceShadow(const Ray& r,
    const float& maxDistance,
    const std::vector<std::shared_ptr<ImplicitShape>>& shapes
    ) {
  constexpr float threshold = 10e-6;
  float t = 0;

  while (t < maxDistance) {
    float minDistance = kInfinity;
    Vec3 from = r.at(t);
    for (auto shape : shapes) {
      float d = shape->getDistance(from);
      if (d < minDistance) {
        minDistance = d;
        if (minDistance <= threshold * t) {
          return true;
        }

      }
    }
    t += minDistance;
  }
  return false;
}


Color shade(
    const Point3& p, const ImplicitShape *shape,
    const std::vector<std::shared_ptr<ImplicitShape>>& shapes,
    const std::vector<std::shared_ptr<Light>>& lights
    ){

  constexpr float delta = 10e-6;

  Vec3 n = Vec3(
        shape->getDistance(p+Vec3(delta,0,0)) - shape->getDistance(p + Vec3(-delta,0,0)),
        shape->getDistance(p+Vec3(0,delta,0)) - shape->getDistance(p + Vec3(0,-delta,0)),
        shape->getDistance(p+Vec3(0,0,delta)) - shape->getDistance(p + Vec3(0,0,-delta))
        );
  n.normalize();

  Color shadeColor = Color(0);

  for (const auto& light : lights) {
    Vec3 lightDir = light->getPosition() - p;
    if (lightDir.dot(n) > 0) {
      float dist2 = lightDir.length2(); // squared dist
      lightDir.normalize();

      // TODO use surface color
      bool shadow = 1 - sphereTraceShadow(Ray(p,lightDir), sqrtf(dist2), shapes);
      shadeColor += shadow * lightDir.dot(n) * light->getColor() * light->getIntensity() /(float) (4 * M_PI * dist2); // with square falloff
    }
  }
  return shadeColor;
}


Color sphereTrace(const Ray& r,
    const std::vector<std::shared_ptr<ImplicitShape>>& shapes,
    const std::vector<std::shared_ptr<Light>>& lights
    ) {
  int tmax = 100;
  float t=0;
  float threshold = 10e-6;
  int n_steps = 0;

  const ImplicitShape *intersectedShape;
  while (t<tmax) {
    float minDistance = kInfinity;

    for (auto shape : shapes) {
      float d = shape->getDistance(r.at(t));
      if (d < minDistance) {
        minDistance = d;
        intersectedShape = shape.get();
      }
    }

    // did we intersect the shape?
    if (minDistance <= threshold * t) {
      return shade(r.at(t), intersectedShape, shapes, lights); // use lights
      //return intersectedShape->color_;          // use only surf color
      //return color(0, float(n_steps)/tmax, 0);  // color by pixel comput cost
    } 
    t += minDistance; 
    n_steps++;
  }
  return Color(0);
}

int main() {
  // Image
  const auto aspect_ratio = 16.0/9.0;
  const int image_width  = 400;
  const int image_height = static_cast<int>(image_width / aspect_ratio);

  Color image[image_width][image_height]; // TODO create class or smthing

  // Scene
  //auto scene = makeScene();
  auto shapes = makeShapes();
  auto lights = makeLights();

  // Camera
  Point3 camera_origin(0, 4, 5);
  Vec3 camera_dir = Vec3(0, 0.5, -1);
  camera_dir.normalize();

  float fov = 45;

  Camera cam(fov, aspect_ratio, camera_origin, camera_dir);

  // Render
  // in img coord (0,0) is top-left 
  for (int j=0; j<image_height; ++j) {
    for (int i=0; i<image_width; ++i) {
      // Put coords in [0,1]
      float u = double(i + .5) / (image_width -1); // NDC Coord
      float v = double(j + .5) / (image_height-1); // NDC Coord

      Ray r = cam.generate_ray(u,v);

      image[i][j] = sphereTrace(r,shapes, lights);
    }
  }

  // ----------------------------------------------------------
  // Write img to file
  // ----------------------------------------------------------
  std::ofstream ofs;
  ofs.open("./imgs/img.ppm");
  ofs << "P3\n" << image_width << " " << image_height << "\n255\n";
  for (int j=0; j<image_height; ++j) {
    for (int i=0; i<image_width; ++i) {
      write_color(ofs, image[i][j]);
    }
  }
  ofs.close();

  // ----------------------------------------------------------
  // SDL
  // ----------------------------------------------------------
  SDL_Event event;
  SDL_Renderer *renderer;
  SDL_Window *window;
  int i;

  SDL_Init(SDL_INIT_VIDEO);
  SDL_CreateWindowAndRenderer(image_width, image_height, 0, &window, &renderer);
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
  SDL_RenderClear(renderer);

  for (int j=0; j<image_height; ++j) {
    for (int i=0; i<image_width; ++i) {
      Color pc = image[i][j];
      SDL_SetRenderDrawColor(renderer,
          pc.r255(),
          pc.g255(),
          pc.b255(),
          255);
      SDL_RenderDrawPoint(renderer, i, j);
    }
  }
  

  SDL_RenderPresent(renderer);
  while (1) {
    if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
      break;
  }
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return EXIT_SUCCESS;
}
