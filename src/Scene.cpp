#include "Scene.h"

// DEBUG STUFF
#include "utilities.h" // for DEBUG_message
#include <string>
#include <iostream>
//#include <chrono>

bool DEBUG_shapes_len = true;

using namespace utilities;
// END // DEBUG STUFF



void Scene::addShape(ImplicitShape* shape) {
  // what if already added? O(n) to look for same val pointer?
  shapes_.push_back(shape);
}
void Scene::addLight(Light* light) {
  lights_.push_back(light);
}
void Scene::addAmbientLight(AmbientLight* light) {
  ambientLight_ = light;
}

void Scene::optimizeScene() {
  DEBUG_message(DEBUG_shapes_len, 
      "shapes_.size() = " + std::to_string(shapes_.size()));

  // collapse all shapes into one big union
  if (shapes_.size() < 2) return;

  DEBUG_message(DEBUG_shapes_len, "optimizing scene...");

  ImplicitShape* big_one = shapes_[0];
  shapes_.erase(shapes_.begin());
  for (auto s : shapes_) {
    big_one = new UnionShape(big_one, s);
  }
  shapes_ = Shapes();
  shapes_.push_back(big_one);

  DEBUG_message(DEBUG_shapes_len, 
      "shapes_.size() = " + std::to_string(shapes_.size()) + "\n");
}


void Scene::set_suggested_ticks(const int ticks) {
  if (ticks > 0) {
    suggested_ticks_ = ticks;
  }
}

void Scene::update() {
  for (auto s : shapes_) {
    s->update();
  }
  // below for moving lights
  //for (auto l : lights) {
  //  l->update();
  //}
}

