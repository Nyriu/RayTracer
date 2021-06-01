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

    void addShape(ImplicitShape* shape);
    void addLight(Light* light);
    void addAmbientLight(AmbientLight* light);

    void optimizeScene();

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

    void set_suggested_ticks(const int ticks);
    int get_suggested_ticks() { return suggested_ticks_; }


    void update();
};


#endif
