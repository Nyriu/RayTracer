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


// /// OLD
// Color BRDF_Specular_GGX_Environment( const Vec3& viewDir, const Vec3& normal, const Color& specularColor, const float& roughness) {
//   float dotNV = clamp(normal.dot(viewDir), 0.0, 1.0 );
// 
//   const Vec4 c0 = Vec4( - 1, - 0.0275, - 0.572, 0.022 );
//   const Vec4 c1 = Vec4( 1, 0.0425, 1.04, - 0.04 );
// 
//   Vec4 r = roughness * c0 + c1;
// 
//   float a004 = std::min( r.x() * r.x(), std::exp2f( - 9.28 * dotNV ) ) * r.x() + r.y();
// 
//   //Vec3 brdf = Vec3( -1.04, 1.04, 0 ) * a004 + r.zw;
//   Vec3 brdf = Vec3( -1.04, 1.04, 0 ) * a004 + Vec3(r.z(), r.w(), 0);
// 
//   return specularColor * brdf.x() + brdf.y();
// }
// 
// Color FSchlick(const float& vDoth, const Color& f0) {
//   return f0 + (Color(1.0) - f0)*pow(1.0 - vDoth,5.0);
// }
// 
// float DGGX(float NoH, float alpha) {
//   float alpha2 = alpha * alpha;
//   float k = NoH*NoH * (alpha2 - 1.0) + 1.0;
//   return alpha2 / (M_PI * k * k );
// }
// 
// float G1(float nDotv, float alpha) {
//   float alpha2 = alpha*alpha;
//   return 2.0 * (nDotv / (nDotv + std::sqrt(alpha2 + (1.0-alpha2)*nDotv*nDotv )));
// }
// 
// float GSmith(float nDotv, float nDotl, float alpha) {
//   DEBUG_message(DEBUG_general, "GSmith :" + std::to_string(
//         G1(nDotl,alpha)*G1(nDotv,alpha)
//         ));
//   return G1(nDotl,alpha)*G1(nDotv,alpha);
// }
// /// END // OLD

Color fschlick(const float& angle, const Color& cspec) { // fresnel approximation
  float angle_pos = clamp_lower(angle, 0);
  return cspec + (Color(1.0) - cspec)*pow(1.0 - angle_pos,5.0);
}

//float g1(const float& nDots, const float& alpha) { // Smith G_1 approximation by Karis
//  return 2 * nDots / (nDots * (2-alpha) + alpha);
//
//}

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

  //Color shadeColor = Color(0);
  Color outRadiance = Color(0);

  // TODO init all things inside for here
  //bool shadow;
  //float dist2 = 0;

  ////float metalness, roughness, alpha = 0;
  //Color cdiff, cspec, fresnel, brdf = Color(0);
  //Vec3 h = Vec3(0);
  //float nDotv, nDoth, vDoth, nDotl = 0;

  Vec3 lightDir(0);
  float nDotl = 0;
  float nDotv = 0;
  float nDoth = 0;
  //float vDotl = 0;
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
      //outRadiance += shape->getColor() * light->getColor() * nDotl; // Lambertian // M_PI canceled out

      //vDotl = viewDir.dot(lightDir);
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
  //int n_steps = 0;

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

    //std::cout << "minDistance = " << minDistance << std::endl;
    // did we intersect the shape?
    if (minDistance <= hit_threshold_ * t) {

      //std::cout << "Color a pixel" << std::endl;
      auto col = 
      shade(r.at(t), r.direction(), intersectedShape);  // use lights
      //std::cout << col << std::endl;
      return col;

      //return intersectedShape->color_;        // use only surf color
      //return Color(float(n_steps)/tmax,0, 0); // color by pixel comput cost
      //return Color(1,0,0);                    // fixed color
    }
    t += minDistance;
    //n_steps++;
  }
  return Color(0);
  //return Color(1,0,0);
  //return Color(0.2,.2,.7);
}

