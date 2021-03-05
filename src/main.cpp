//#include <algorithm>
//#include <fstream>
//#include <cmath>
//#include <functional>
#include <iostream>
#include <vector>
#include <limits>

#include "color.h"
#include "vec3.h"
#include "mat3.h"
#include "mat4.h"
#include "ray.h"
#include "camera.h"

//#include "ImplicitShape.h" // TODO use later

constexpr float kInfinity = std::numeric_limits<float>::max(); 

float evalImplicitFunction(const point3 &p) {
  //sphere
  point3 c(0, 0, 0);
  float r = 1;
  float d = length(p-c) - r;

  //float d = std::min(
  //    length(p) - 1.f,
  //    length(p-point3(0,1,0)) - 0.5f
  //    );
  return d;
}


color ray_color(const ray& r) {
  int tmax = 100;
  float t=0;
  float threshold = 10e-6;
  int n_steps = 0;
  while (t<tmax) {
    float minDistance = kInfinity;

    //float d = std::min(1.f, std::pow(std::fabs(evalImplicitFunction(r.at(t))), 0.3f));
    //float d = std::min(1.f, std::fabs(evalImplicitFunction(r.at(t))));
    //float d = std::fabs(evalImplicitFunction(r.at(t)));
    float d = evalImplicitFunction(r.at(t));

    if (d < minDistance) {
      minDistance = d;
    }
    // did we intersect the shape?
    if (minDistance <= threshold * t) {
      return color(0, float(n_steps)/tmax, 0); 
    } 
    t += minDistance; 
    n_steps++;
  }
  vec3 unit_direction = normalize(r.direction());
  float k = 0.5*(unit_direction.y + 1.0);
  return (1.f-k)*color(1.0) + k*color(0.5,0.7,1.0);
}

int main() {
  // Image
  const auto aspect_ratio = 16.0/9.0;
  const int image_width  = 400;
  //const int image_width  = 16;
  const int image_height = static_cast<int>(image_width / aspect_ratio);
  const float scale = 1.f;

  //const int image_width  = 16;
  //const int image_height = 9;

  //const int image_width  = 400;
  //const int image_height = 400;

  std::vector<color> image; // TODO create class or smthing

  // Camera
  point3 origin(0, 0, 3);
  //point3 origin(2, 0, 3);
  //vec3 camera_dir(0, 0, -1);
  vec3 camera_dir(0, 0, -1);

  //float fov = 45;
  float fov = 90;
  //float fov = 180;

  camera cam(fov, aspect_ratio, origin, camera_dir);
  //cam.translate(0,1,0);

  //cam.put_at(vec3(3,3,0));
  //cam.put_at(0,0,3);

  //cam.look_at(vec3(0,0,1));
  //cam.look_at(0,0,1);
  //cam.look_at(-1,0,0);

  // Render
  // in img coord (0,0) is top-left 
  for (int j=0; j<image_height; ++j) {
    for (int i=0; i<image_width; ++i) {
      // Put coords in [0,1]
      float u = double(i + .5) / (image_width -1); // NDC Coord
      float v = double(j + .5) / (image_height-1); // NDC Coord

      ray r = cam.generate_ray(u,v);

      image.push_back(ray_color(r));
    }
  }
  //ray_color(ray(origin, vec3(0,0,1))); // DEBUG

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



