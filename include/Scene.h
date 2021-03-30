#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <memory>

#include "ImplicitShape.h"
#include "Camera.h"
#include "Light.h"

class Scene {
  public:
    // is this good?
    //using Shapes = std::vector<std::shared_ptr<ImplicitShape>>;
    //using Lights = std::vector<std::shared_ptr<Light>>;
    using Shapes = std::vector<ImplicitShape*>;
    using Lights = std::vector<        Light*>;


  private:
    //  TODO shared_ptr no more needed?
    Shapes shapes_;
    Lights lights_;
    AmbientLight* ambientLight_ = nullptr;
    //bool to_update
    //Camera *camera_ = nullptr;
    Camera camera_;
    bool has_camera_ = false;

  public:
    Scene() = default;
    Scene(Shapes shapes, Lights lights) : shapes_(shapes), lights_(lights) {}

    void addShape(ImplicitShape* shape) {
    //void addShape(ImplicitShape& shape) {
      //shapes_.push_back(std::make_shared<ImplicitShape>(shape));
      shapes_.push_back(shape);
    }
    //void addLight(Light& light) {
    //  lights_.push_back(std::make_shared<Light>(light));
    void addLight(Light* light) {
      lights_.push_back(light);
    }
    void addAmbientLight(AmbientLight* light) {
      ambientLight_ = light;
    }

    bool hasAmbientLight() const { return ambientLight_ != nullptr; }
    bool hasCamera() const { return has_camera_; }
    //bool hasCamera() const { return camera_ != nullptr; }
    //void addCamera(Camera* camera) { camera_ = camera; }
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
};


#endif
