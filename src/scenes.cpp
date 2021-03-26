#include "scenes.h"

Scene makeScene_1() {
  Scene scene;
  // Lights
  scene.addLight(new PointLight(Point3(4,3,3), Color(1), 200));
  scene.addLight(new PointLight(Point3(0,-3,3), Color(.8,.2,.5), 50));
  scene.addLight(new PointLight(Point3(0,0,-3), Color(.3,.8,.3), 20));

  // Shapes
  //scene.addShape(new Sphere(Point3(0),1.5));
  scene.addShape(new Sphere(Point3(0),1.5, Color(.5,.5,.5)));

  // Camera
  Point3 camera_origin(0,0,5);

  Point3 view(0, -1, 0);

  float fov = 45;
  Camera cam(fov, camera_origin, view);
  scene.addCamera(cam);

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


Scene makeScene_ShadowDebug() {
  Scene scene;
  // Lights
  scene.addLight(new PointLight(Point3(4,3,4), Color(1), 200));

  //scene.addLight(new PointLight(Point3(4,0,3), Color(1), 200));
  //scene.addLight(new PointLight(Point3(-4,0,3), Color(1), 200));
  //scene.addLight(new PointLight(Point3(0, 4,0), Color(1), 200));
  //scene.addLight(new PointLight(Point3(0,-4,0), Color(1), 200));

  // Shapes
  scene.addShape(
      new UnionShape(
      //new SubtractShape(
        new Sphere(Point3(0),2, Color(1,0,0)),
  //);
  //scene.addShape(
        new Sphere(Point3(1,0,2),.5, Color(0,1,0))
        )
      );

  // Camera
  Point3 camera_origin(0,0,5);
  Point3 view(0, 0, 0);

  float fov = 45;
  Camera cam(fov, camera_origin, view);
  scene.addCamera(cam);

  return scene;
}

