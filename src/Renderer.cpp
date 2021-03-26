#include "Renderer.h"

Renderer Renderer::setScene(Scene* scene) {
  scene_ = scene;
  tracer_->setScene(scene);

  if (!hasCamera()) {
    if (!scene->hasCamera()) {
      // TODO
      // throw error and ask for a camera!
      std::cerr << "Neither the Scene nor the Renderer has a Camera!" << std::endl;
      std::cerr << "Noooooow! Segfault!" << std::endl;
      return nullptr;
    }
    setCamera(scene->getCamera());
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
  //generateFrame(shapes, lights);
  //img_.writePPM("./imgs/img.ppm");

  if (!no_window) {
    if (!win_->isOpen()) {
      std::cout << "open win" << std::endl;
      win_->openWindow();
    }
  }
  mainLoop();
}


void Renderer::generateFrame() {
  //std::cout << *cam_ << std::endl;

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
  //while (cam_.isToUpdate() && win_.keepRendering())
  int t_ = 0;
  if (!no_window) {
    while (win_->keepRendering()) {
      //cam_->translate(Vec3(t_,0,0));
      //std::cout << "main loop" << std::endl;
      //cam_.isToUpdate();
      generateFrame();
      win_->drawImage(img_);
      //img_.writePPM("./imgs/img.ppm");
      t_++;
    }
  } else {
    //const int tmax_ = 10;
    const int tmax_ = 1;
    std::string prefix = "./wip_imgs/time_";
    //std::string prefix = "./wip_imgs/seq_";
    std::string suffix = ".ppm";
    while (t_ < tmax_) {
      //cam_.translate(Vec3(-2 + t_,0,0));
      cam_->translate(Vec3(t_,0,0));
      //std::cout << "main loop" << std::endl;
      generateFrame();
      //img_.writePPM(prefix + std::to_string(t_) + suffix);
      img_.writePPM(prefix + std::to_string(std::time(0)) + suffix);

      t_++;
    }
  }
}
