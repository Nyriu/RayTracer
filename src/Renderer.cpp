#include "Renderer.h"


// DEBUG STUFF
#include "utilities.h" // for DEBUG_message
#include <string>
#include <iostream>
#include <chrono>
#define DEBUG_FRAME_INFO

using namespace utilities;

// END // DEBUG STUFF

Renderer Renderer::setScene(Scene* scene) {
  scene_ = scene;
  tracer_->setScene(scene);

  if (!hasCamera()) {
    if (!scene->hasCamera()) {
      // throw error and ask for a camera!
      std::cerr << "Neither the Scene nor the Renderer has a Camera!" << std::endl;
      exit(1);
    }
    setCamera(scene->getCamera());
  }
  return *this;
}

Renderer Renderer::setScene(Scene* scene, bool use_suggested_max_ticks) {
  setScene(scene);
  if (use_suggested_max_ticks) {
    max_num_ticks_ = scene->get_suggested_ticks();
  }
  return *this;
}

Renderer Renderer::setCamera(Camera* camera) {
  cam_ = camera;
  cam_->setAspectRatio(win_->aspect_ratio);
  return *this;
}


void Renderer::render() {
  // check if scene exists
  if (!no_window_) {
    if (!win_->isOpen()) {
      std::cout << "open win" << std::endl;
      win_->openWindow();
    }
  }
  optimizeScene(); // verify to not create uselessly the octree twice
  mainLoop();
}

void Renderer::optimizeScene() {
  if (scene_optimized_ && !use_octree_) return;
  if (!scene_optimized_)
    scene_->optimizeScene();
  if (use_octree_)
    tracer_->setScene(scene_);
  scene_optimized_ = true;
}


void Renderer::generateFrame() {
  // in img coord (0,0) is top-left
  for (int j=0; j<img_.height; ++j) {
    for (int i=0; i<img_.width; ++i) {
      // Put coords in [0,1]
      float u = double(i + .5) / (img_.width -1); // NDC Coord
      float v = double(j + .5) / (img_.height-1); // NDC Coord

      Ray r = cam_->generate_ray(u,v);

      img_.setPixel(tracer_->trace(r), i,j);
    }
  }
}

void Renderer::mainLoop() {
  if (!no_window_) {
    while (win_->keepRendering()) {
      if (current_tick_ < max_num_ticks_) {
        //qui contare tempo per frame
        //https://stackoverflow.com/questions/22387586/measuring-execution-time-of-a-function-in-c
#ifdef DEBUG_FRAME_INFO
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        std::cout <<
          "generating frame num " <<
          current_tick_ << "\n" << std::endl;
#endif

        scene_->update();
        generateFrame();
        current_tick_++;

#ifdef DEBUG_FRAME_INFO
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        std::cout << "Frame Gen Time = " << std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() << "[s]" << std::endl;
        std::cout << "Frame Gen Time = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;
        //std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << std::endl;
#endif

      }
      win_->drawImage(img_);
    }
  } else {
    std::string prefix = "./wip_imgs/time_";
    //std::string prefix = "./wip_imgs/seq_";
    std::string suffix = ".ppm";
    while (current_tick_ < max_num_ticks_) {
      std::cout << "generating frame num " << current_tick_ << "\n" << std::endl;
      scene_->update();
      generateFrame();
      img_.writePPM(prefix + std::to_string(std::time(0)) + suffix);
      current_tick_++;
    }
  }
}
