#include "Tracer.h"

// DEBUG STUFF
#include "utilities.h" // for DEBUG_message
#include <cmath>
#include <string>
bool DEBUG_sphereTraceShadow = false;
bool DEBUG_general = true;

using namespace utilities;

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

      // Self-Hit Shadowing Error

      //// First method // Problems where actualy should be in shadow
      //if (shape == shapeToShadow && d < 10e-7)
      ////if (shape == shapeToShadow)
      //  continue;

      // Second method // Best so far
      if (shape == shapeToShadow && d <= hit_threshold_ * t) {
        // move "from" a bit over the surface (along the normal direction)
        d = shape->getDistance(
            from + shape->getNormalAt(from) * 10e-7
            );
      }

      // Third method
      // Try to descend into the CSG and exclude only current shape


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

Color Tracer::shade(const Point3& p, const Vec3& viewDir, const ImplicitShape *shape) {
  Vec3 n = shape->getNormalAt(p);

  Color outRadiance = Color(0);

  Vec3 lightDir, h;
  float nDotl, nDotv, nDoth, vDoth;
  Color brdf;

  bool shadow;
  float dist2 = 0;

  Color cdiff = shape->getAlbedo(p);
  float shininess_factor = shape->getShininess(p);
  Color cspec = shape->getSpecular(p);

  for (const auto& light : scene_->getLights()) {
    lightDir = (light->getPosition() - p);
    dist2 = lightDir.length2(); // squared dist
    lightDir.normalize();
    nDotl = n.dot(lightDir);

    if (nDotl > 0) {
      h = (viewDir+lightDir).normalize();
      vDoth = viewDir.dot(h);
      nDotv = viewDir.dot(h);
      nDoth = n.dot(h);

      Vec3 r = 2 * nDotl * n - lightDir;
      float vDotr = viewDir.dot(r);

      brdf =
        cdiff / M_PI +
        cspec * powf(vDotr, shininess_factor);

      // With shadows below
      shadow = sphereTraceShadow(Ray(p,lightDir), shape);
      outRadiance += (1-shadow) * brdf * light->getColor() * light->getIntensity() * nDotl
        / (float) (4 * dist2) // with square falloff
        ;
    }
  }
  if (scene_->hasAmbientLight()) {
    Light* ambientLight = scene_->getAmbientLight();
    outRadiance += ambientLight->getColor() * ambientLight->getIntensity() * cdiff;
  }
  return outRadiance;
}


Color Tracer::sphereTrace(const Ray& r) {
  float t=0;

  const ImplicitShape *intersectedShape;
  while (t < max_distance_) {
    float minDistance = infinity;

    for (auto shape : scene_->getShapes()) {
      float d = shape->getDistance(r.at(t));
      if (d < minDistance) {
        minDistance = d;
        intersectedShape = shape;
      }
    }

    // did we intersect the shape?
    if (minDistance <= hit_threshold_ * t) {
      return shade(r.at(t), r.direction(), intersectedShape);  // use lights
      //return intersectedShape->color_;        // use only surf color
      //return Color(float(n_steps)/tmax,0, 0); // color by pixel comput cost
      //return Color(1,0,0);                    // fixed color
    }
    t += minDistance;
  }
  return Color(0); // bg color // no hit color
  //return Color(1,0,0);
  //return Color(0.2,.2,.7);
}

