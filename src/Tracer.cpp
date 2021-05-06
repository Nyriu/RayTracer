#include "Tracer.h"

// DEBUG STUFF
#include "utilities.h" // for DEBUG_message
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
      //if (shape == shapeToShadow && d < anti_selfhit_shadow_threshold_)
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


Color fschlick(const float& angle, const Color& cspec) { // fresnel approximation
  float angle_pos = clamp_lower(angle, 0);
  return cspec + (Color(1.0) - cspec)*pow(1.0 - angle_pos,5.0);
}


float gsmith(const float& nDotv, const float& nDotl, const float& alpha) { // Hammon approx
  return 0.5 / (
      lerp(
        2 * abs(nDotl) * abs(nDotv),
        abs(nDotl) + nDotv,
        alpha
        )
      );
}



Color Tracer::shade(const Point3& p, const Vec3& viewDir, const ImplicitShape *shape) {
  Vec3 n = shape->getNormalAt(p);

  Color outRadiance = Color(0);

  Vec3 lightDir(0);
  float nDotl = 0;
  float nDotv = 0;
  float nDoth = 0;
  Vec3 h = Vec3(0);
  float vDoth = 0;
  Color cspec(0);
  Color cdiff(0);
  Color brdf(0);
  Color fresnel(0);
  float roughness = 0;
  float alpha = 0;

  bool shadow;
  float dist2 = 0;

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

      // Plastic example
      cdiff = shape->getColor();
      cspec = Color(0.04);
      roughness = 0;

      // // Metal example
      // cdiff = Color(0);
      // cspec = shape->getColor();
      // roughness = 0;

      alpha = roughness * roughness;

      //fresnel = fschlick(vDoth, cspec);
      brdf =
        cdiff / M_PI //+                       // diffuse
        //(Color(1) - fresnel) * cdiff / M_PI //+                       // diffuse
        //fresnel // / (4.0*abs(nDotv)*abs(nDotl));
        //fresnel * gsmith(nDotv, nDotl, alpha) * DGGX(nDoth,alpha)
      ; // specular

      //outRadiance += brdf * light->getColor() * nDotl;
      //outRadiance += brdf * light->getColor() * light->getIntensity() * nDotl;

      // With shadows below
     shadow = sphereTraceShadow(Ray(p,lightDir), shape);
     outRadiance += (1-shadow) * brdf * light->getColor() * light->getIntensity() * nDotl
       / (float) (4 * dist2) // with square falloff
       ;

    //shadeColor += (1-shadow) * lightDir.dot(n) * light->getColor() * light->getIntensity()
    }

  }

  cdiff = shape->getColor();
  if (scene_->hasAmbientLight()) {
    //std::cout << "hasAmbientLight" << std::endl;
    //outRadiance += ambientLight*cdiff; // TODO ambient lights
    //outRadiance += Color(0.15) * cdiff; // TODO ambient lights
    Light* ambientLight = scene_->getAmbientLight();
    outRadiance += ambientLight->getColor() * ambientLight->getIntensity() * cdiff; // TODO ambient lights
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

