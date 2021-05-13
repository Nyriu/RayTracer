#include "Renderer.h"

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
  if (!no_window) {
    if (!win_->isOpen()) {
      std::cout << "open win" << std::endl;
      win_->openWindow();
    }
  }
  mainLoop();
}


void Renderer::generateFrame() {
  if (cam_->update()) {
    // in img coord (0,0) is top-left
    for (int j=0; j<img_.height; ++j) {
      for (int i=0; i<img_.width; ++i) {
        // Put coords in [0,1]
        float u = double(i + .5) / (img_.width -1); // NDC Coord
        float v = double(j + .5) / (img_.height-1); // NDC Coord

        Ray r = cam_->generate_ray(u,v);

        img_.setPixel(tracer_->sphereTrace(r), i,j);
      }
    }
  }
}

void Renderer::mainLoop() {
  if (!no_window) {
    while (win_->keepRendering()) {
      scene_->update();
      generateFrame();
      win_->drawImage(img_);
      if (current_tick_ < max_num_ticks_)
        current_tick_++;
    }
  } else {
    std::string prefix = "./wip_imgs/time_";
    //std::string prefix = "./wip_imgs/seq_";
    std::string suffix = ".ppm";
    while (current_tick_ < max_num_ticks_) {
      //std::cout << "generating frame num " << current_tick_ << "\n" << std::endl;
      scene_->update();
      generateFrame();
      img_.writePPM(prefix + std::to_string(std::time(0)) + suffix);
      current_tick_++;
    }
  }
}
