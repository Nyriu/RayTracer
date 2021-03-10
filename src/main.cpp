//#include <algorithm>
//#include <fstream>
//#include <cmath>
//#include <functional>
#include <iostream>
#include <vector>
#include <limits>
#include <memory> // shared_ptr // todo use
 // https://en.cppreference.com/w/cpp/memory/shared_ptr

#include "color.h"
#include "vec3.h"
#include "mat3.h"
#include "mat4.h"
#include "ray.h"
#include "camera.h"

#include "ImplicitShape.h"
#include "Light.h"


constexpr float kInfinity = std::numeric_limits<float>::max(); 

bool sphereTraceShadow(const ray& r,
    const float& maxDistance,
    std::vector<std::shared_ptr<ImplicitShape>>& shapes
    ) {
  constexpr float threshold = 10e-6;
  float t = 0;

  while (t < maxDistance) {
    float minDistance = kInfinity;
    vec3 from = r.at(t);
    for (auto shape : shapes) {
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


color shade(
    const point3& p, const ImplicitShape *shape,
    std::vector<std::shared_ptr<ImplicitShape>>& shapes,
    const std::vector<const Light*> lights
    ){

  constexpr float delta = 10e-6;

  vec3 n = normalize( vec3(
        shape->getDistance(p+vec3(delta,0,0)) - shape->getDistance(p + vec3(-delta,0,0)),
        shape->getDistance(p+vec3(0,delta,0)) - shape->getDistance(p + vec3(0,-delta,0)),
        shape->getDistance(p+vec3(0,0,delta)) - shape->getDistance(p + vec3(0,0,-delta))
        )
      );

  color shadeColor = vec3(0);

  for (const auto& light : lights) {
    vec3 lightDir = light->position_ - p;
    if (glm::dot(lightDir, n) > 0) {
      float dist2 = length2(lightDir); // squared dist
      lightDir = normalize(lightDir);

      // TODO use surface color
      bool shadow = 1 - sphereTraceShadow(ray(p,lightDir), sqrtf(dist2), shapes);
      shadeColor += shadow * glm::dot(lightDir,n) * light->color_ * light->intensity_ /(float) (4 * M_PI * dist2); // with square falloff
    }
  }

  return shadeColor;
}


color sphereTrace(const ray& r,
    std::vector<std::shared_ptr<ImplicitShape>>& shapes,
    //const std::vector<const ImplicitShape *>& shapes,
    const std::vector<const Light *>& lights
    ) {
  int tmax = 100;
  float t=0;
  float threshold = 10e-6;
  int n_steps = 0;

  const ImplicitShape *intersectedShape;
  while (t<tmax) {
    float minDistance = kInfinity;

    for (auto shape : shapes) {
      float d = shape->getDistance(r.at(t));
      if (d < minDistance) {
        minDistance = d;
        intersectedShape = shape.get();
      }
    }

    // did we intersect the shape?
    if (minDistance <= threshold * t) {
      return shade(r.at(t), intersectedShape, shapes, lights); // use lights
      //return intersectedShape->color_;          // use only surf color
      //return color(0, float(n_steps)/tmax, 0);  // highlight comput heavy pixels
    } 
    t += minDistance; 
    n_steps++;
  }
  // simple sky color
  //vec3 unit_direction = normalize(r.direction());
  //float k = 0.5*(unit_direction.y + 1.0);
  //return (1.f-k)*color(1.0) + k*color(0.5,0.7,1.0);
  return color(0);
}


//std::vector<const ImplicitShape *> makeShapes() {
std::vector<std::shared_ptr<ImplicitShape>> makeShapes() { 
  //std::vector<const ImplicitShape *> shapes;

  //shapes.push_back(new Sphere(vec3(0), 1));
  //shapes.push_back(new Sphere(vec3(0.5), 1));
  //shapes.push_back(new Sphere(vec3(0,0,-13), 3));

  //shapes.push_back(new Sphere(vec3(0,.5,0), .5, color(1,0,1)));
  //shapes.push_back(new Sphere(vec3(1,1,0),  1 , color(1,0,0)));
  //shapes.push_back(new Sphere(vec3(2,2,0),  1 , color(0,1,0)));
  //shapes.push_back(new Sphere(vec3(3,3,0),  1 , color(0,0,1)));

  //shapes.push_back(new Torus(point3(0,3,0),1,0.2));
  //shapes.push_back(new Torus(point3(0,1.5,0),1.5,0.4));
  //shapes.push_back(new Torus(2,0.65));


  std::vector<std::shared_ptr<ImplicitShape>> shapes; 
  //shapes.push_back(std::make_shared<Sphere>(vec3(0), 1));


  point3 sph_c(0,1,0.3);
  point3 tor_c(0.5,1,0);

  shapes.push_back(
      std::make_shared<UnionShape>(
        std::make_shared<Sphere>(sph_c, 1),
        std::make_shared<Torus> (tor_c, 1, .2)
        ));
  float off_val = -4;
  vec3 offset1(off_val,0,0);
  shapes.push_back(
      std::make_shared<IntersectShape>(
        std::make_shared<Sphere>(offset1 + sph_c, 1),
        std::make_shared<Torus> (offset1 + tor_c, 1, .2)
        ));
  vec3 offset2(-off_val,0,0);
  shapes.push_back(
      std::make_shared<SubtractShape>(
        std::make_shared<Sphere>(offset2 + sph_c, 1),
        std::make_shared<Torus> (offset2 + tor_c, 1, .2)
        ));




  //shapes.push_back(new Cube(point3(.25)));

  // terrain
  //shapes.push_back(new Sphere(vec3(0,-100,0), 100, color(.5)));

  return shapes;
}

std::vector<const Light *> makeLights() {
  std::vector<const Light *> lights;

  lights.push_back(new PointLight(point3(3,5,-2), color(1), 400) );
  lights.push_back(new PointLight(point3(2,2,5), color(1), 100) );
  //lights.push_back(new PointLight(point3(10,10,0), color(1), 4000) );
  //lights.push_back(new PointLight(point3(2,2,3), color(.8, .8, 0), 30) );

  return lights;
}

int main() {
  // Image
  const auto aspect_ratio = 16.0/9.0;
  const int image_width  = 300;
  //const int image_width  = 400;
  //const int image_width  = 900;
  //const int image_width  = 1800;
  //const int image_width  = 16;
  const int image_height = static_cast<int>(image_width / aspect_ratio);
  const float scale = 1.f;

  //const int image_width  = 16;
  //const int image_height = 9;

  //const int image_width  = 400;
  //const int image_height = 400;

  std::vector<color> image; // TODO create class or smthing

  // Scene
  //auto scene = makeScene();
  auto shapes = makeShapes();
  auto lights = makeLights();

  // Camera
  point3 camera_origin(0, 4, 5);
  //point3 camera_origin(0, 4, 8);
  //point3 camera_origin(0, 2, 7);
  //point3 camera_origin(1,1,3);
  //vec3 camera_dir = normalize(vec3(0, 0, -1));
  vec3 camera_dir = normalize(vec3(0, 0.5, -1));
  //vec3 camera_dir = normalize(vec3(0, 0.2, -1));
  //vec3 camera_dir = normalize(vec3(0, -1, -1));

  //point3 camera_origin(0, 0, -10);
  //vec3 camera_dir = normalize(vec3(0, -.2, 1));

  float fov = 45;
  //float fov = 90;
  //float fov = 180;

  camera cam(fov, aspect_ratio, camera_origin, camera_dir);

  //cam.translate(0,1,0);
  //cam.put_at(vec3(3,3,0));
  //cam.look_at(vec3(0,0,1));

  // Render
  // in img coord (0,0) is top-left 
  for (int j=0; j<image_height; ++j) {
    for (int i=0; i<image_width; ++i) {
      // Put coords in [0,1]
      float u = double(i + .5) / (image_width -1); // NDC Coord
      float v = double(j + .5) / (image_height-1); // NDC Coord

      ray r = cam.generate_ray(u,v);

      //image.push_back(ray_color(r));
      image.push_back(sphereTrace(r,shapes, lights));
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



