//#include <algorithm>
//#include <fstream>
//#include <cmath>
#include <iostream>
#include <vector>
//#include <limits>

#include "color.h"
#include "vec3.h"
#include "mat3.h"
#include "mat4.h"
#include "ray.h"

//#include "ImplicitShape.h" // TODO use later

//float evalImplicitFunction(point3 p) {
//  //sphere
//  //float d = p.length_squared() - 10000.f;
//  //point3 c(0.5, 0.5, -1.3);
//  point3 c(0.5, 0.5, -1);
//  float r = 1;
//  //float d = (p.x() - c.x())*(p.x() - c.x()) + 
//  //          (p.y() - c.y())*(p.y() - c.y()) + 
//  //          (p.z() - c.z())*(p.z() - c.z()) - r;
//  point3 q = p - c;
//  float d = q.length_squared() - r;
//  std::cout << "p " << p << std::endl;
//  std::cout << "distance " << d << "\n" << std::endl;
//  return d;
//}
//
//constexpr float kInfinity = std::numeric_limits<float>::max(); 
//
//color ray_color(const ray& r) {
//  std::cout << "---" << std::endl;
//  int tmax = 100;
//  float t=0;
//  //float threshold = 10e-6;
//  float threshold = 10e-3;
//  int n_steps = 0;
//  while (t<tmax) {
//    float minDistance = kInfinity;
//
//    std::cout << "t " << t << std::endl;
//    //std::cout << t << std::endl;
//    //float d = std::min(1.f, std::pow(std::fabs(evalImplicitFunction(r.at(t))), 0.3f));
//    //float d = std::min(1.f, std::fabs(evalImplicitFunction(r.at(t))));
//    float d = std::fabs(evalImplicitFunction(r.at(t)));
//    if (d < minDistance) {
//      minDistance = d;
//    }
//    // did we intersect the shape?
//    if (minDistance <= threshold * t) { // threshold lin with distance
//      //return color(n_steps); 
//      std::cout << "color!" << std::endl;
//      return color(1,0,0);
//    } 
//    t += minDistance; 
//    n_steps++;
//  }
//  //return color(0);
//  vec3 unit_direction = unit_vector(r.direction());
//  auto k = 0.5*(unit_direction.y() + 1.0);
//  return (1.0-k)*color(1.0) + k*color(0.5,0.7,1.0);
//}
//
////// This is not SDF!
////bool hit_sphere(const point3& center, double radius, const ray& r) {
////  vec3 oc = r.origin() - center;
////  auto a = dot(r.direction(), r.direction());
////  auto b = 2.0 * dot(oc, r.direction());
////  auto c = dot(oc, oc) - radius*radius;
////  auto discriminant = b*b - 4*a*c;
////  return(discriminant > 0);
////}
//
////color ray_color(const ray& r) {
////  //if (hit_sphere(point3(0,0,-1), 0.5, r))
////  //  return color(1,0,0);
////  vec3 unit_direction = unit_vector(r.direction());
////  auto t = 0.5*(unit_direction.y() + 1.0);
////  return (1.0-t)*color(1.0) + t*color(0.5,0.7,1.0);
////}

color ray_color(const ray& r) {
  //if (hit_sphere(point3(0,0,-1), 0.5, r))
  //  return color(1,0,0);
  vec3 unit_direction = normalize(r.direction());
  float t = 0.5*(unit_direction.y + 1.0);
  return (1.f-t)*color(1.0) + t*color(0.5,0.7,1.0);
}

int main() {
  // Image
  //const auto aspect_ratio = 16.0/9.0;
  //const int image_width  = 400;
  //const int image_height = static_cast<int>(image_width / aspect_ratio);
  //const float scale = 2.f;

  const int image_width  = 16;
  const int image_height = 9;
  std::vector<color> image;

  // Camera
  //auto viewport_height = 2.0;
  //auto viewport_width  = aspect_ratio * viewport_height;
  ////std::cout << "viewport_width:" << viewport_width << std::endl;
  ////std::cout << "viewport_height:" << viewport_height << std::endl;
  //auto focal_length = 1.0;

  point3 origin(0);
  //std::cout << "origin " <<  origin << std::endl;
  //vec3 horizontal(viewport_width, 0, 0);
  //vec3 vertical(0, viewport_height, 0);
  //auto lower_left_corner = origin - horizontal/2.f - vertical/2.f - vec3(0,0,focal_length);
  //std::cout << "lower_left_corner " <<  lower_left_corner << std::endl;



  // Render
  // in img coord (0,0) is top-left 
  //for (int j=image_height-1; j>=0; --j) {
  for (int j=0; j<image_height; ++j) {
    for (int i=0; i<image_width; ++i) {
      //color pixelColor(double(i)/(image_width-1), double(j)/(image_height-1), 0.25);
      //image.push_back(pixelColor);

      //std::cout << j << " " << i << std::endl;

      // Put coords in [0,1]
      float u = double(i + .5) / (image_width -1); // NDC Coord
      float v = double(j + .5) / (image_height-1); // NDC Coord


      // Put coords in [-1,1]
      float su = 2 * u - 1; // Screen Coord
      float sv = 1 - 2 * v; // Screen Coord (flip y axis)

      // TODO Aspect Ratio
      // TODO Field Of View

      // From ScreenCoords to WorldCoords
      // for now camera-to-world matrix its identity
      // TODO Move the camera from the origin!
      // move to world coord and cast ray
      float wu = su;
      float wv = sv;
      ray r(origin, vec3(wu,wv,-1));

      image.push_back(ray_color(r));


      //float delta  = 0.007;

      //ray r(origin, lower_left_corner + u*horizontal + v*vertical - origin); // TODO check formula
      //image.push_back(ray_color(r));

      //if (0.5 - delta < u  && u < 0.5 + delta && 0.5 - delta < v  && v < 0.5 + delta) {
      //  //image.push_back(color(0,1,0));
      //  //std::cout << "bg" << std::endl;
      //  ray r(origin, lower_left_corner + u*horizontal + v*vertical - origin); // TODO check formula
      //  image.push_back(ray_color(r));
      //} else {
      //image.push_back(color(0.5));
      //}
    }
  }

  // Write img to file
  std::ofstream ofs;
  ofs.open("./imgs/img.ppm");
  ofs << "P3\n" << image_width << " " << image_height << "\n255\n";
  for (auto pixelColor : image) {
    write_color(ofs, pixelColor);
  }
  ofs.close();

  return 0;

}


//int main() {
//  std::cout << "Let's restruct things" << std::endl;
//
//  std::cout << vec3(1) << std::endl;
//  std::cout << color(1) << std::endl;
//  std::cout <<
//    mat3(
//        1,2,3,
//        4,5,6,
//        7,8,9
//        )
//    << std::endl;
//  std::cout <<
//    mat4(
//        1,2,3,4,
//        5,6,7,8,
//        9,10,11,12,
//        13,14,15,16
//        )
//    << std::endl;
//
//  vec3 v1(1,0,0);
//  vec3 v2(0,1,0);
//  vec3 v3(0,0,1);
//
//  std::cout << v1 * v2 << std::endl;
//  std::cout << mat3(v1,v2,v1) << std::endl;
//
//  return 0;
//}



