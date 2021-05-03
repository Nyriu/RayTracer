#include "scenes.h"

Scene makeScene_1() {
  float intensity_scale = .02;

  Scene scene;
  // Lights
  scene.addLight(new PointLight(Point3(4,3,3), Color(1), 200 * intensity_scale));
  //scene.addLight(new PointLight(Point3(0,-3,3), Color(.8,.2,.5), 50));
  //scene.addLight(new PointLight(Point3(0,0,-3), Color(.3,.8,.3), 20));
  //scene.addLight(new PointLight(Point3(0,-3,3), Color(1), 50 * intensity_scale));

  // Shapes
  //scene.addShape(new Sphere(Point3(0),1.5));
  scene.addShape(new Sphere(Point3(0),1.5, Color(.9,.2,.2)));

  // Camera
  Point3 camera_origin(0,0,5);

  Point3 view(0, -1, 0);

  float fov = 45;
  //Camera cam(fov, camera_origin, view);
  Camera cam;
  cam.translate(camera_origin.as_Vec3());
  cam.lookAt(0,0,0);
  scene.addCamera(cam);

  return scene;
}

Scene makeScene_Spheres() {
  Scene scene;
  // Lights
  scene.addLight(new PointLight(Point3(5,5,-2), Color(1), 200));
  scene.addLight(new PointLight(Point3(2,2,5), Color(1),  200));
  scene.addAmbientLight(new AmbientLight(Color(1), 0.7));

  // Shapes
  scene.addShape(new Sphere(Point3(0,.5,0),.5));
  scene.addShape(new Sphere(Point3(1,1, 0),1));
  scene.addShape(new Sphere(Point3(2,2, 0),1));
  scene.addShape(new Sphere(Point3(3,3, 0),1));

  scene.addShape(new Sphere(Point3(0,-100,0),100)); // Terrain

  // Camera
  Point3 camera_origin(2,3,7);
  Vec3 camera_dir = Vec3(0, 0.2, -1);
  camera_dir.normalize();
  float fov = 45;
  //Camera cam(fov, camera_origin, camera_dir);
  Camera cam;
  cam.translate(camera_origin.as_Vec3());
  cam.lookAt(0,0,0);
  scene.addCamera(cam);

  return scene;
}

Scene makeScene_Toruses() {
  Scene scene;
  // Lights
  //scene.addLight(new PointLight(Point3(0,0,0), Color(1), 250));
  scene.addLight(new PointLight(Point3(3,7,-2), Color(1), 600));
  scene.addAmbientLight(new AmbientLight(Color(1), .07));

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
  //Camera cam(fov, camera_origin, camera_dir);
  Camera cam;
  cam.translate(camera_origin.as_Vec3());
  cam.lookAt(0,0,0);
  scene.addCamera(cam);

  return scene;
}


Scene makeScene_CSG_1() {
  Scene scene;
  // Lights
  scene.addLight(new PointLight(Point3(3,3,2), Color(1), 600));
  scene.addAmbientLight(new AmbientLight(Color(0,.3,0.3), 1));

  scene.addShape(
      new SubtractShape(
        new UnionShape(
          new Torus(Point3(0,1.5,0), .5,.2),
          new UnionShape(
            new Torus(1.5,0.4),
            new Torus(Point3(0,-1.5,0), .5,.2))
          ),
        new Sphere(1.7)
        ) 
      );
  // Camera
  Point3 camera_origin(0,5,4.4);
  Point3 view(0, 10, 0);
  float fov = 45;
  //Camera cam(fov, camera_origin, view);
  Camera cam;
  cam.translate(camera_origin - Point3(0,1,0));
  cam.lookAt(1,1,0);
  scene.addCamera(cam);

  return scene;
}


Scene makeScene_CSG_2() {
  Scene scene;
  // Lights
  scene.addLight(new PointLight(Point3(4,3,2), Color(1), 400));
  scene.addAmbientLight(new AmbientLight(Color(0,.3,0.3), 1));

  float x_offset = -2;
  scene.addShape(
      new SubtractShape(
        new Sphere(Point3(x_offset, 0,0), 1.7),
        new UnionShape(
          new Torus(Point3(x_offset,1.5,0), .5,.2),
          new UnionShape(
            new Torus(Point3(x_offset,0,0), 1.5,0.4),
            new Torus(Point3(x_offset,-1.5,0), .5,.2))
          )
        )
      );

  x_offset = -x_offset;
  scene.addShape(
      new IntersectShape(
        new Sphere(Point3(x_offset, 0,0), 1.7),
        new UnionShape(
          new Torus(Point3(x_offset,1.5,0), .5,.2),
          new UnionShape(
            new Torus(Point3(x_offset,0,0), 1.5,0.4),
            new Torus(Point3(x_offset,-1.5,0), .5,.2))
          )
        )
      );


  // Camera
  Point3 camera_origin(0,3,5);
  Point3 view(0, 6, 0);
  float fov = 45;
  //Camera cam(fov, camera_origin, view);
  Camera cam;
  cam.translate(camera_origin - Point3(0));
  cam.lookAt(0,0,0);
  scene.addCamera(cam);

  return scene;
}


Scene makeScene_CSG_3() {
  Scene scene;
  // Lights
  scene.addLight(new PointLight(Point3(4,3,2), Color(1,.3,.3), 400));
  scene.addLight(new PointLight(Point3(-4,-3,2), Color(.3,1,.3), 400));
  scene.addAmbientLight(new AmbientLight(Color(1), .07));

  float bigger_r = 2;
  float smaller_r= 0.5;

  scene.addShape(
      new Sphere(Point3(0,0,0), bigger_r, Color(.7,.2,.2))
      );
  scene.addShape(
      new Sphere(Point3(0,0,bigger_r), smaller_r, Color(.2,.7,.2))
      );
  scene.addShape(
      new Sphere(
        Vec3(2,2,2).normalize().as_Point() * (bigger_r - 0.2), smaller_r, Color(.2,.2,.7))
      );
  scene.addShape(
      new Sphere(
        Vec3(-2,2,2).normalize().as_Point() * (bigger_r - 0.2), smaller_r, Color(.2,.2,.7))
      );


  // Camera
  Point3 camera_origin(0,3,5);
  Point3 view(0, 6, 0);
  float fov = 45;
  //Camera cam(fov, camera_origin, view);
  Camera cam;
  cam.translate(camera_origin - Point3(0));
  cam.lookAt(0,0,0);
  scene.addCamera(cam);

  return scene;
}



// Scene makeScene_ShadowDebug() {
//   Scene scene;
//   // Lights
//   scene.addLight(new PointLight(Point3(4,3,3.5), Color(1), 200));
// 
//   //scene.addLight(new PointLight(Point3(4,0,3), Color(1), 200));
//   //scene.addLight(new PointLight(Point3(-4,0,3), Color(1), 200));
//   //scene.addLight(new PointLight(Point3(0, 4,0), Color(1), 200));
//   //scene.addLight(new PointLight(Point3(0,-4,0), Color(1), 200));
// 
//   // Shapes
//   scene.addShape(
//       new UnionShape(
//         new SubtractShape(
//           new Sphere(Point3(0),2, Color(1,0,0)),
//           new Sphere(Point3(1,0,2),.5, Color(0,1,0))
//           ),
//         new Sphere(Point3(.5,1,1.7),.5, Color(0,1,1))
//         )
//       );
// 
//   // Camera
//   Point3 camera_origin(0,0,5);
//   Point3 view(0, 0, 0);
// 
//   float fov = 45;
//   Camera cam(fov, camera_origin, view);
//   scene.addCamera(cam);
// 
//   return scene;
// }
// 
// 
// Scene makeScene_TransformDebug() {
//   float intensity_scale = 100;
// 
//   Scene scene;
//   // Lights
//   scene.addLight(new PointLight(Point3(2,2,0), Color(1), 100 * intensity_scale));
//   scene.addAmbientLight(new AmbientLight(Color(1), .5));
// 
//   // Shapes
//   //ImplicitShape *sp = new Sphere(Point3(1,1,0), 1, Color(1));
//   //ImplicitShape *sp = new Sphere(1, Color(1));
//   //sp->translate(Vec3(.5,.5,0));
//   //sp->translate(Vec3(.5,.5,0));
//   //scene.addShape(sp);
// 
// 
//   //ImplicitShape *to = new Torus(Point3(0.5,0.5,0), 1, 0.2);
//   //to->translate(Vec3(.75,.75,0));
//   //to->translate(Vec3(-.25,-.55,0));
//   //to->rotateX(degree_to_radians(20));
//   ////to->rotateY(degree_to_radians(90));
//   //to->rotateZ(degree_to_radians(50));
//   //scene.addShape(to);
// 
//   ImplicitShape *to1 = new Torus(.5, 0.1);
//   to1->setColor(1,0,0);
//   to1->translate(.5,.5,.5);
//   to1->rotateX(45);
//   to1->rotateY(45);
//   to1->rotateZ(45);
//   scene.addShape(to1);
// 
// 
//   // Camera
//   Point3 camera_origin(0,0,3);
// 
//   //Point3 view(0, 0, -1);
//   Vec3 view(0,0,-1);
// 
//   float fov = 45;
//   Camera cam(fov, camera_origin, view);
//   scene.addCamera(cam);
// 
//   return scene;
// }


Scene makeScene_CameraTransformDebug() {
  float intensity_scale = 100;

  Scene scene;
  // Lights
  scene.addLight(new PointLight(Point3(-10,10,-3), Color(1), 20 * intensity_scale));
  //scene.addLight(new PointLight(Point3(0,0,0), Color(1), 2 * intensity_scale));
  //scene.addLight(new PointLight(Point3(4,-4,-3), Color(1), 100 * intensity_scale));
  scene.addAmbientLight(new AmbientLight(Color(1), .6));

  // Shapes
  //scene.addShape(
  //    new Sphere(Point3(3,3,0), .3, Color(1,1,0))
  //    );
  //ImplicitShape *to1 = new Torus(.5, 0.1);
  //to1->setColor(1,0,0);
  //to1->translate(0,0,2);
  //to1->rotateX(45);
  //to1->rotateY(45);
  ////to1->rotateZ(45);
  //scene.addShape(to1);

  //ImplicitShape *to2 = new Torus(.5, 0.1);
  //to2->setColor(0,1,0);
  //to2->translate(0,0,-2);
  //to2->rotateX(-45);
  //to2->rotateY(-45);
  ////to2->rotateZ(45);
  //scene.addShape(to2);

  scene.addShape( new Sphere(Point3(0), .2, Color(0,1,1)));
  ImplicitShape *to3 = new Torus(.7, 0.1);
  to3->setColor(.3,.3,1);
  //to3->translate(0,0.2,0);
  to3->rotateX(45);
  to3->rotateY(30);
  scene.addShape(to3);

  ImplicitShape *to4 = new Torus(.4, 0.11);
  to4->setColor(.7,.3,.3);
  //to4->translate(0,0.2,0);
  to4->rotateX(10);
  to4->rotateY(10);
  scene.addShape(to4);


  // Camera
  //Point3 camera_origin(0,0,0);
  ////Point3 view(0, 0, -1);
  //Vec3 view(0,0,1);
  //float fov = 45;

  Camera cam;
  int k=2;
  Point3 cam_pos(k,k,0);
  cam.translate(cam_pos.as_Vec3());
  cam.lookAt(0,0,0);
  //cam.lookAt(0,0,-2);
  //cam.lookAt(3,3,0);
  //cam.rotateY(180);
  //cam.rotateX(-20);
  //cam.rotateZ(20);
  scene.addCamera(cam);

  return scene;
}



Scene makeScene_HierarchyTransformDebug() {
  float intensity_scale = 100;

  Scene scene;
  // Lights
  scene.addLight(new PointLight(Point3(-10,10,-3), Color(1), 20 * intensity_scale));
  //scene.addLight(new PointLight(Point3(0,0,0), Color(1), 2 * intensity_scale));
  //scene.addLight(new PointLight(Point3(4,-4,-3), Color(1), 100 * intensity_scale));
  scene.addAmbientLight(new AmbientLight(Color(1), .6));

  // Shapes
  ImplicitShape *sp = new Sphere(.5); // not added to scene // works as general 3D obj
  float r = .5; // radius
  float t = .1; // thickness
  ImplicitShape *to1 = (new Torus(r,t));
  to1 -> setColor(.9,.4,.4);
  to1 -> translate(-1.f/2.f*r,0,0);
  //to1 -> rotateX(10);
  //to1 -> rotateY(10);
  //to1 -> rotateZ(10);
  to1->setParent(sp);
  scene.addShape(to1);

  ImplicitShape *to2 = (new Torus(r,t));
  to2 -> setColor(.4,.4,.9);
  to2 -> translate(1.f/2.f*r,0,0);
  to2 -> rotateX(90);
  //to2 -> rotateY(10);
  //to2 -> rotateZ(10);
  to2->setParent(sp);
  scene.addShape(to2);

  float rot=45;
  rot+=15;
  rot+=15;
  rot+=15;
  rot+=15;
  rot+=15;
  rot+=15;
  rot+=15;
  rot+=15;
  rot+=15;
  sp->rotateX(rot);



  // Camera
  //Point3 camera_origin(0,0,0);
  ////Point3 view(0, 0, -1);
  //Vec3 view(0,0,1);
  //float fov = 45;

  Camera cam;
  Point3 cam_pos(0,0,2);
  cam.translate(cam_pos.as_Vec3());
  cam.lookAt(0,0,0);
  scene.addCamera(cam);

  return scene;
}


Scene makeScene_HierarchyCSGTransformDebug() {
  Scene scene;
  // Lights
  scene.addLight(new PointLight(Point3(4,3,2), Color(1), 400));
  scene.addAmbientLight(new AmbientLight(Color(0,.3,0.3), 1));

  float x_offset = 0;
  ImplicitShape *csg1 =
    new IntersectShape(
        new Sphere(Point3(x_offset, 0,0), 1.7),
        new UnionShape(
          new Torus(Point3(x_offset,1.5,0), .5,.2),
          new UnionShape(
            new Torus(Point3(x_offset,0,0), 1.5,0.4),
            new Torus(Point3(x_offset,-1.5,0), .5,.2))
          )
        );



  float rot=45;
  rot-=15;
  rot-=15;
  rot-=15;
  csg1->rotateX(rot);
  csg1->rotateY(rot);
  scene.addShape(csg1);


  // Camera
  Point3 camera_origin(0,3,5);
  Point3 view(0, 6, 0);
  float fov = 45;
  //Camera cam(fov, camera_origin, view);
  Camera cam;
  cam.translate(camera_origin - Point3(0));
  cam.lookAt(0,0,0);
  scene.addCamera(cam);

  return scene;
}



