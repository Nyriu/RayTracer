#ifndef RENDERER_H
#define RENDERER_H


// TODO remove useless imports
#include <exception>
#include <iostream>
#include <string>
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
#include "Scene.h"

// SDL
#include <stdlib.h>
#include <SDL2/SDL.h>

constexpr float kInfinity = std::numeric_limits<float>::max();


class Renderer {
  private:
    Window *win_ = nullptr;
    Camera *cam_ = nullptr;

    //temporarly and img instead of a window
    Image img_;

    bool no_window = false;

  private:
    // Scene stuff
    const Scene* scene_ = nullptr;


  public:
    Renderer() = default;

    Renderer(Window* window) :
      win_(window), img_(win_->width, win_->aspect_ratio) { }

    Renderer(Window* window, Camera* camera) :
      win_(window), cam_(camera), img_(win_->width, win_->aspect_ratio) {
        cam_->setAspectRatio(win_->aspect_ratio);
      }

    bool hasCamera() { return cam_ != nullptr; }
    Renderer setScene(Scene* scene);
    Renderer setCamera(Camera* camera);

    bool sphereTraceShadow( const Ray& r, const float& maxDistance, const Scene* scene);
    Color shade( const Point3& p, const ImplicitShape *shape, const Scene* scene);
    Color sphereTrace(const Ray& r, const Scene* scene);

    void render();
    void generateFrame();
    void mainLoop();



    void disableWindow(){
      this->no_window = true;
    }
    void enableWindow(){
      this->no_window = false;
    }

};

#endif
