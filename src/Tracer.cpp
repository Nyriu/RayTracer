#include "Tracer.h"

// DEBUG STUFF
#include "utilities.h" // for DEBUG_message
bool DEBUG_sphereTraceShadow = false;


// END // DEBUG STUFF




bool Tracer::sphereTraceShadow(const Ray& r, const ImplicitShape *shapeToShadow) {
  bool &D = DEBUG_sphereTraceShadow;

  DEBUG_message(D,
      "[-------------------------\ninside sphereTraceShadow"
      );

  float t = 0;
  while (t < max_distance_) {
    float min_distance = infinity;
    Point3 from = r.at(t);
    for (auto shape : scene_->getShapes()) {
      float d = shape->getDistance(from);

      // First method // Problems where actualy should be in shadow
      //if (shape == shapeToShadow && d < anti_selfhit_shadow_threshold_)
      //  continue;

      // Second method
      if (shape == shapeToShadow && d <= hit_threshold_ * t) {
        // move "from" a bit over the surface (along the normal direction)
        d = shape->getDistance(
            from + shape->getNormalAt(from) * 10e-7
            );
      // Third method // Try to descend into the CSG and exclude only current shape

      }

      if (d < min_distance) {
        min_distance = d;
        if (min_distance <= hit_threshold_ * t) {
  DEBUG_message(D,
      "exiting sphereTraceShadow (true)\n-------------------------]\n"
      );
          return true;
        }
      }
    }
    t += min_distance;
  }

  DEBUG_message(D,
      "exiting sphereTraceShadow (false)\n-------------------------]\n"
      );

  return false;
}


Color Tracer::shade(const Point3& p, const ImplicitShape *shape) {
  Vec3 n = shape->getNormalAt(p);

  Color shadeColor = Color(0);

  for (const auto& light : scene_->getLights()) {
    //std::cout << "light" << std::endl;
    Vec3 lightDir = light->getPosition() - p;
    if (lightDir.dot(n) > 0) {
      float dist2 = lightDir.length2(); // squared dist
      lightDir.normalize();

      // TODO use surface color
      //bool shadow = 1 - sphereTraceShadow(Ray(p,lightDir), sqrtf(dist2), scene);
      bool shadow = sphereTraceShadow(Ray(p,lightDir), shape);
      // // DEBUG CODE
      // if (shadow){
      //   DEBUG_sphereTraceShadow = true;
      //   sphereTraceShadow(Ray(p,lightDir), shape);
      //   DEBUG_sphereTraceShadow = false;
      // }
      // // END // DEBUG CODE

      // TODO HERE USE SHAPE
      //shadeColor += (1-shadow) * lightDir.dot(n) * light->getColor() * light->getIntensity() /(float) (4 * M_PI * dist2); // with square falloff
      shadeColor += (1-shadow) * lightDir.dot(n) * shape->getColor(p) * light->getColor();
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
  //return Color(0);
  //return Color(1,0,0);
  return Color(0.2,.2,.7);
}

