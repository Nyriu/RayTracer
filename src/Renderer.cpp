#include "Renderer.h"

Renderer Renderer::setScene(Scene* scene) {
  scene_ = scene;
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





bool Renderer::sphereTraceShadow(const Ray& r,
    const float& maxDistance,
    const Scene* scene
    ) {
  constexpr float threshold = 10e-6;
  float t = 0;

  while (t < maxDistance) {
    float minDistance = kInfinity;
    Point3 from = r.at(t);
    for (auto shape : scene->getShapes()) {
      float d = shape->getDistance(from);
      if (d < minDistance) {
        minDistance = d;
        if (minDistance <= threshold * t) {
          return true;
        }

      }
    }
    t += minDistance;
  }
  return false;
}


Color Renderer::shade(
    const Point3& p, const ImplicitShape *shape,
    const Scene* scene
    ){

  constexpr float delta = 10e-6;

  Vec3 n = Vec3(
      shape->getDistance(p+Vec3(delta,0,0)) - shape->getDistance(p + Vec3(-delta,0,0)),
      shape->getDistance(p+Vec3(0,delta,0)) - shape->getDistance(p + Vec3(0,-delta,0)),
      shape->getDistance(p+Vec3(0,0,delta)) - shape->getDistance(p + Vec3(0,0,-delta))
      );
  n.normalize();

  Color shadeColor = Color(0);

  for (const auto& light : scene->getLights()) {
    //std::cout << "light" << std::endl;
    Vec3 lightDir = light->getPosition() - p;
    if (lightDir.dot(n) > 0) {
      float dist2 = lightDir.length2(); // squared dist
      lightDir.normalize();

      // TODO use surface color
      bool shadow = 1 - sphereTraceShadow(Ray(p,lightDir), sqrtf(dist2), scene);
      shadeColor += shadow * lightDir.dot(n) * light->getColor() * light->getIntensity() /(float) (4 * M_PI * dist2); // with square falloff
    }
  }
  return shadeColor;
}


Color Renderer::sphereTrace(const Ray& r, const Scene* scene) {
  int tmax = 100;
  float t=0;
  float threshold = 10e-6;
  int n_steps = 0;

  const ImplicitShape *intersectedShape;
  while (t<tmax) {
    float minDistance = kInfinity;

    for (auto shape : scene->getShapes()) {
      float d = shape->getDistance(r.at(t));
      if (d < minDistance) {
        //std::cout << "intersected shape" << std::endl;
        minDistance = d;
        intersectedShape = shape;
      }
    }

    // did we intersect the shape?
    if (minDistance <= threshold * t) {
      //std::cout << "Color a pixel" << std::endl;
      return shade(r.at(t), intersectedShape, scene); // use lights
      //return intersectedShape->color_;                // use only surf color
      //return Color(float(n_steps)/tmax,0, 0);         // color by pixel comput cost
      //return Color(1,0,0);                            // fixed color
    }
    t += minDistance;
    n_steps++;
  }
  return Color(0);
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

        img_.setPixel(sphereTrace(r,scene_), i,j);
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
    std::string prefix = "./wip_imgs/seq_";
    std::string suffix = ".ppm";
    while (t_ < tmax_) {
      //cam_.translate(Vec3(-2 + t_,0,0));
      cam_->translate(Vec3(t_,0,0));
      //std::cout << "main loop" << std::endl;
      generateFrame();
      img_.writePPM(prefix + std::to_string(t_) + suffix);
      t_++;
    }
  }
}
