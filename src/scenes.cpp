#include "scenes.h"

////std::vector<const ImplicitShape *> makeShapes() {
//std::vector<std::shared_ptr<ImplicitShape>> makeShapes1() {
//  //std::vector<const ImplicitShape *> shapes;
//
//  //shapes.push_back(new Sphere(vec3(0), 1));
//  //shapes.push_back(new Sphere(vec3(0.5), 1));
//  //shapes.push_back(new Sphere(vec3(0,0,-13), 3));
//
//  //shapes.push_back(new Sphere(vec3(0,.5,0), .5, color(1,0,1)));
//  //shapes.push_back(new Sphere(vec3(1,1,0),  1 , color(1,0,0)));
//  //shapes.push_back(new Sphere(vec3(2,2,0),  1 , color(0,1,0)));
//  //shapes.push_back(new Sphere(vec3(3,3,0),  1 , color(0,0,1)));
//
//  //shapes.push_back(new Torus(point3(0,3,0),1,0.2));
//  //shapes.push_back(new Torus(point3(0,1.5,0),1.5,0.4));
//  //shapes.push_back(new Torus(2,0.65));
//
//
//  std::vector<std::shared_ptr<ImplicitShape>> shapes;
//  //shapes.push_back(std::make_shared<Sphere>(vec3(0), 1));
//
//  Point3 sph_c(0,1,0.3);
//  Point3 tor_c(0.5,1,0);
//
//  shapes.push_back(
//      std::make_shared<UnionShape>(
//        std::make_shared<Sphere>(sph_c, 1),
//        std::make_shared<Torus> (tor_c, 1, .2)
//        ));
//  float off_val = -4;
//  Vec3 offset1(off_val,0,0);
//  shapes.push_back(
//      std::make_shared<IntersectShape>(
//        std::make_shared<Sphere>(offset1 + sph_c, 1),
//        std::make_shared<Torus> (offset1 + tor_c, 1, .2)
//        ));
//  Vec3 offset2(-off_val,0,0);
//  shapes.push_back(
//      std::make_shared<SubtractShape>(
//        std::make_shared<Sphere>(offset2 + sph_c, 1),
//        std::make_shared<Torus> (offset2 + tor_c, 1, .2)
//        ));
//
//
//  //shapes.push_back(new Cube(point3(.25)));
//
//  // terrain
//  //shapes.push_back(new Sphere(vec3(0,-100,0), 100, color(.5)));
//
//  return shapes;
//}
//
//Lights makeLights1() {
//  Lights lights;
//
//  lights.push_back(std::make_shared<PointLight>(Point3(3,5,-2), Color(1), 400) );
//  lights.push_back(std::make_shared<PointLight>(Point3(2,2,5), Color(1), 100) );
//  //lights.push_back(new PointLight(point3(10,10,0), color(1), 4000) );
//  //lights.push_back(new PointLight(point3(2,2,3), color(.8, .8, 0), 30) );
//
//  return lights;
//}

Scene makeScene_1() {
  Scene scene;
  // Lights
  scene.addLight(new PointLight(Point3(4,3,3), Color(1), 200));
  scene.addLight(new PointLight(Point3(0,-3,3), Color(.8,.2,.5), 50));

  // Shapes
  scene.addShape(new Sphere(Point3(0),1.5));

  // Camera
  Point3 camera_origin(-.5, 1.5, 6);
  Vec3 camera_dir = Vec3(0, 0.5, -1);
  camera_dir.normalize();
  float fov = 45;
  Camera cam(fov, camera_origin, camera_dir);

  //scene.addCamera(&cam);

  return scene;
}

Scene makeScene_Spheres() {
  Scene scene;
  // Lights
  scene.addLight(new PointLight(Point3(5,5,-2), Color(1), 400));
  scene.addLight(new PointLight(Point3(2,2,5), Color(1), 100));

  // Shapes
  scene.addShape(new Sphere(Point3(0,.5,0),.5));
  scene.addShape(new Sphere(Point3(1,1,0),1));
  scene.addShape(new Sphere(Point3(2,2,0),1));
  scene.addShape(new Sphere(Point3(3,3,0),1));

  scene.addShape(new Sphere(Point3(0,-100,0),100)); // Terrain

  // Camera
  Point3 camera_origin(2,3,7);
  Vec3 camera_dir = Vec3(0, 0.2, -1);
  camera_dir.normalize();
  float fov = 45;
  Camera cam(fov, camera_origin, camera_dir);
  scene.addCamera(cam);

  return scene;
}


Scene makeScene_Toruses() {
  Scene scene;
  // Lights
  //scene.addLight(new PointLight(Point3(0,3.5,0), Color(1), 40));
  scene.addLight(new PointLight(Point3(3,7,-2), Color(1), 400));

  // Shapes
  scene.addShape(new Torus(Point3(0,3,0),1,0.2));
  scene.addShape(new Torus(Point3(0,1.5,0),1.5,0.4));
  scene.addShape(new Torus(2,0.65));

  scene.addShape(new Sphere(Point3(0,-102,0),100)); // Terrain


  // Camera
  Point3 camera_origin(0,6,6);
  Vec3 camera_dir = Vec3(0, 0.7, -1);
  camera_dir.normalize();
  float fov = 45;
  Camera cam(fov, camera_origin, camera_dir);
  scene.addCamera(cam);

  return scene;
}


