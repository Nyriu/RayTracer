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


      // DEBUG STUFF
      bool debug_enabled = false;
      int target_i = -1; int target_j = -1; // on target
      //target_i = 190; target_j = 229;
      //target_i = 187; target_j = 0;
      //target_i = 280; target_j = 120;
      //target_i = 158; target_j = 239-50;
      //target_i = 170; target_j = 182;
      //target_i = 168; target_j = 239-61;
      //target_i = 138; target_j = 119; // TODO
      if (debug_enabled || (i == target_i && j == target_j))
        std::cout <<
          "\n--------------------------------------------" << std::endl;
      // END // DEBUG STUFF
      Color c = tracer_->trace(r);

      // DEBUG STUFF
      if (debug_enabled || (-1 != target_i && j != -1)) {
        std::cout <<
          "\npixel coords = [" << i << ", " << j << "]" <<
          "\npixel NDC = [" << u << ", " << v << "]" <<
          "\ncolor = " << c <<
          std::endl;
      }
      if (i == target_i && j == target_j) {
        std::cout <<
          "\nGOT TARGET!!" <<
          "\ntarget pixel coords = [" << target_i << ", " << target_j << "]" <<
          std::endl;
        exit(1);
      }
      // END // DEBUG STUFF

      img_.setPixel(c, i,j);
    }
  }
}

void Renderer::mainLoop() {
  if (!no_window_) {
    while (win_->keepRendering()) {
      if (current_tick_ < max_num_ticks_) {
        //qui contare tempo per frame
        //https://stackoverflow.com/questions/22387586/measuring-execution-time-of-a-function-in-c
        scene_->update();
#ifdef DEBUG_FRAME_INFO
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        std::cout <<
          "generating frame num " <<
          current_tick_ << "\n" << std::endl;
#endif
        generateFrame();
#ifdef DEBUG_FRAME_INFO
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        std::cout << "Frame Gen Time = " << std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() << "[s]" << std::endl;
        std::cout << "Frame Gen Time = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;
        //std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << std::endl;
#endif
        current_tick_++;
      }
      win_->drawImage(img_);
    }
  } else {
    std::string prefix = "./wip_imgs/time_";
    //std::string prefix = "./wip_imgs/seq_";
    std::string suffix = ".ppm";
    while (current_tick_ < max_num_ticks_) {
      scene_->update();
#ifdef DEBUG_FRAME_INFO
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        std::cout <<
          "generating frame num " <<
          current_tick_ << "\n" << std::endl;
#endif
      generateFrame();
#ifdef DEBUG_FRAME_INFO
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        std::cout << "Frame Gen Time = " << std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() << "[s]" << std::endl;
        std::cout << "Frame Gen Time = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;
        //std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << std::endl;
#endif
      img_.writePPM(prefix + std::to_string(std::time(0)) + suffix);
      current_tick_++;
    }
  }
}
