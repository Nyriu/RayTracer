#include "Tracer.h"


bool Tracer::sphereTraceShadow(const Ray& r) {
  float t = 0;
  while (t < max_distance_) {
    float min_distance = infinity;
    Point3 from = r.at(t);
    for (auto shape : scene_->getShapes()) {
      float d = shape->getDistance(from);
      if (d < min_distance) {
        min_distance = d;
        if (min_distance <= hit_threshold_ * t) {
          return true;
        }

      }
    }
    t += min_distance;
  }
  return false;
}


Color Tracer::shade(const Point3& p, const ImplicitShape *shape) {
  Vec3 n = Vec3(
      shape->getDistance(
        p+Vec3(gradient_delta_,0,0)) - shape->getDistance(p + Vec3(-gradient_delta_,0,0)),
      shape->getDistance(
        p+Vec3(0,gradient_delta_,0)) - shape->getDistance(p + Vec3(0,-gradient_delta_,0)),
      shape->getDistance(
        p+Vec3(0,0,gradient_delta_)) - shape->getDistance(p + Vec3(0,0,-gradient_delta_))
      );
  n.normalize();

  Color shadeColor = Color(0);

  for (const auto& light : scene_->getLights()) {
    //std::cout << "light" << std::endl;
    Vec3 lightDir = light->getPosition() - p;
    if (lightDir.dot(n) > 0) {
      float dist2 = lightDir.length2(); // squared dist
      lightDir.normalize();

      // TODO use surface color
      //bool shadow = 1 - sphereTraceShadow(Ray(p,lightDir), sqrtf(dist2), scene);
      bool shadow = 1 - sphereTraceShadow(Ray(p,lightDir));

      // TODO HERE USE SHAPE
      //shadeColor += shadow * lightDir.dot(n) * light->getColor() * light->getIntensity() /(float) (4 * M_PI * dist2); // with square falloff
      shadeColor += lightDir.dot(n) * shape->getColor() * light->getColor(); // light->getIntensity() /(float) (4 * M_PI * dist2); // with square falloff
    }
  }
  return shadeColor;
}


Color Tracer::sphereTrace(const Ray& r) {
  float t=0;
  int n_steps = 0;

  const ImplicitShape *intersectedShape;
  while (t < max_distance_) {
    float minDistance = infinity;

    for (auto shape : scene_->getShapes()) {
      float d = shape->getDistance(r.at(t));
      if (d < minDistance) {
        //std::cout << "intersected shape" << std::endl;
        minDistance = d;
        intersectedShape = shape;
      }
    }

    // did we intersect the shape?
    if (minDistance <= hit_threshold_ * t) {
      //std::cout << "Color a pixel" << std::endl;
      return shade(r.at(t), intersectedShape);  // use lights
      //return intersectedShape->color_;        // use only surf color
      //return Color(float(n_steps)/tmax,0, 0); // color by pixel comput cost
      //return Color(1,0,0);                    // fixed color
    }
    t += minDistance;
    //n_steps++;
  }
  return Color(0);
}

