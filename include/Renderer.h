#ifndef RENDERER_H
#define RENDERER_H


// TODO remove useless imports
#include <iostream>
#include <vector>
#include <limits>
#include <memory> // shared_ptr // todo use
// https://en.cppreference.com/w/cpp/memory/shared_ptr

#include "Color.h"
#include "geometry.h"
#include "Ray.h"
#include "Camera.h"
#include "Image.h"
#include "Window.h"

#include "ImplicitShape.h"
#include "Light.h"

// SDL
#include <stdlib.h>
#include <SDL2/SDL.h>

#include "scenes.h"

constexpr float kInfinity = std::numeric_limits<float>::max();


class Renderer {
  private:
    Window win_;
    Camera cam_;
    // tracer
    // scene

    //temporarly and img instead of a window
    Image img_;
  public:
    //Renderer(Image& img) : img_(img) {}
    //Renderer(const int image_width, const float aspect_ratio) :
    //  img_(image_width, aspect_ratio), win_(image_width, aspect_ratio) {}
    Renderer(const Window& window, const Camera& camera) :
      win_(window), cam_(camera), img_(win_.width, win_.aspect_ratio) {}

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


    void render() {
      // Scene
      //auto scene = makeScene();
      auto shapes = makeShapes();
      auto lights = makeLights();

      //Image img_(win_.width, win_.aspect_ratio);


      // Render
      // in img coord (0,0) is top-left
      for (int j=0; j<img_.height; ++j) {
        for (int i=0; i<img_.width; ++i) {
          // Put coords in [0,1]
          float u = double(i + .5) / (img_.width -1); // NDC Coord
          float v = double(j + .5) / (img_.height-1); // NDC Coord

          Ray r = cam_.generate_ray(u,v);

          img_.setPixel(sphereTrace(r,shapes, lights), i,j);
        }
      }

      img_.writePPM("./imgs/img.ppm");
      win_.drawImage(img_);

      while (win_.keepRendering()) {
        //img_.writePPM("./imgs/img.ppm");
        //win_.drawImage(img_);
      }
    }

};










#endif
