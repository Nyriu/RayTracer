#include "scenes.h"

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

  Point3 sph_c(0,1,0.3);
  Point3 tor_c(0.5,1,0);

  shapes.push_back(
      std::make_shared<UnionShape>(
        std::make_shared<Sphere>(sph_c, 1),
        std::make_shared<Torus> (tor_c, 1, .2)
        ));
  float off_val = -4;
  Vec3 offset1(off_val,0,0);
  shapes.push_back(
      std::make_shared<IntersectShape>(
        std::make_shared<Sphere>(offset1 + sph_c, 1),
        std::make_shared<Torus> (offset1 + tor_c, 1, .2)
        ));
  Vec3 offset2(-off_val,0,0);
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

std::vector<std::shared_ptr<Light>> makeLights() {
  std::vector<std::shared_ptr<Light>> lights;

  lights.push_back(std::make_shared<PointLight>(Point3(3,5,-2), Color(1), 400) );
  lights.push_back(std::make_shared<PointLight>(Point3(2,2,5), Color(1), 100) );
  //lights.push_back(new PointLight(point3(10,10,0), color(1), 4000) );
  //lights.push_back(new PointLight(point3(2,2,3), color(.8, .8, 0), 30) );

  return lights;
}
