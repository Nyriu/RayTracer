#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <memory>

#include "ImplicitShape.h"
#include "Camera.h"
#include "Light.h"

class Scene {
  public:
    using Shapes = std::vector<ImplicitShape*>;
    using Lights = std::vector<        Light*>;


  private:
    Shapes shapes_;
    Lights lights_;
    AmbientLight* ambientLight_ = nullptr;
    Camera camera_;
    bool has_camera_ = false;

    int suggested_ticks_ = 1; // note for the renderer about how many frame to render // always > 0
                              // useful if looping scene


  public:
    Scene() = default;
    Scene(Shapes shapes, Lights lights) : shapes_(shapes), lights_(lights) {}

    void addShape(ImplicitShape* shape) {
      // what if already added? O(n) to look for same val pointer?
      shapes_.push_back(shape);
    }
    void addLight(Light* light) {
      lights_.push_back(light);
    }
    void addAmbientLight(AmbientLight* light) {
      ambientLight_ = light;
    }

    bool hasAmbientLight() const { return ambientLight_ != nullptr; }
    bool hasCamera() const { return has_camera_; }
    void addCamera(Camera camera) {
      camera_ = camera;
      has_camera_ = true;
    }

    //addLight()
    Shapes getShapes() const { return shapes_; }
    Lights getLights() const { return lights_; }
    Light* getAmbientLight() const { return ambientLight_; }
    //Camera* getCamera() const { return camera_; }
    Camera* getCamera() { return &camera_; }

    void set_suggested_ticks(const int ticks) {
      if (ticks > 0) {
        suggested_ticks_ = ticks;
      }
    }
    int get_suggested_ticks() { return suggested_ticks_; }


    void update() {
      for (auto s : shapes_) {
        s->update();
      }
      // below for moving lights
      //for (auto l : lights) {
      //  l->update();
      //}
    }
};


#endif
