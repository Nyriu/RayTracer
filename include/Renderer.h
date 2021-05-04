#ifndef RENDERER_H
#define RENDERER_H


// TODO remove useless imports
#include <exception>
#include <iostream>
#include <string>
#include <vector>
#include <limits>
//#include <memory> // shared_ptr // todo use not
// https://en.cppreference.com/w/cpp/memory/shared_ptr
#include<ctime>

#include "Color.h"
#include "geometry.h"
#include "Ray.h"
#include "Camera.h"
#include "Image.h"
#include "Window.h"

#include "ImplicitShape.h"
#include "Light.h"
#include "Scene.h"
#include "Tracer.h"

// SDL
#include <stdlib.h>
#include <SDL2/SDL.h>


class Renderer {
  private:
    Window *win_ = nullptr;
    Camera *cam_ = nullptr;

    bool no_window = false;

    //temporarly and img instead of a window
    Image img_;

    // Scene stuff
    Scene *scene_ = nullptr;

    //Tracer *tracer_ = nullptr;
    Tracer *tracer_ = new Tracer();

    int current_tick_  = 0;
    int max_num_ticks_ = 1; // maximum number of ticks before stopping update of scene

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
    Renderer setScene(Scene* scene, bool use_suggested_max_ticks);
    Renderer setCamera(Camera* camera);
    Renderer setMaxNumTicks(const int max_num_ticks) {
      max_num_ticks_ = max_num_ticks;
      return *this;
    }

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
