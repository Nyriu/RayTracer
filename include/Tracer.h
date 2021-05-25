#ifndef TRACER_H
#define TRACER_H


// TODO remove useless imports
#include <exception>
#include <iostream>
#include <string>
#include <vector>
#include <limits>

#include "Color.h"
#include "geometry.h"
#include "Ray.h"
#include "Camera.h"
#include "Image.h"
#include "Window.h"

#include "ImplicitShape.h"
#include "Light.h"
#include "Scene.h"

// SDL
#include <stdlib.h>
#include <SDL2/SDL.h>

class Tracer {
  private:
    // Scene stuff
    const Scene* scene_ = nullptr;

    // Tracer Options
    int   max_distance_  = 100; // max n of steps along a ray // draw distance?
    float hit_threshold_ = 10e-6; // min distance to signal a ray-sruface hit
    //float anti_selfhit_shadow_threshold_ = 10e-15;



  public:
    Tracer() = default;
    Tracer(Scene* scene) : scene_(scene){ }

    bool hasScene() { return scene_ != nullptr; }
    Tracer setScene(Scene* scene) {
      scene_ = scene;
      return *this;
    }

    Color sphereTrace(const Ray& r); // better with pointer?
    Color shade(const Point3& p, const Vec3& viewDir, const ImplicitShape *shape);
    bool sphereTraceShadow(const Ray& r, const ImplicitShape *shapeToShadow);

};


#endif
