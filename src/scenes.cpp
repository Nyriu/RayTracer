#include "scenes.h"
#include <cmath>
#include <iostream>

Scene makeScene_1() {
  float intensity_scale = 1;

  Scene scene;
  // Lights
  scene.addLight(new PointLight(Point3(4,3,3), Color(1), 200 * intensity_scale));
  //scene.addLight(new PointLight(Point3(0,-3,3), Color(.8,.2,.5), 50));
  //scene.addLight(new PointLight(Point3(0,0,-3), Color(.3,.8,.3), 20));
  //scene.addLight(new PointLight(Point3(0,-3,3), Color(1), 50 * intensity_scale));
  scene.addAmbientLight(new AmbientLight(Color(1), 0.15));

  // Shapes
  //scene.addShape(new Sphere(Point3(0),1.5));
  scene.addShape(new Sphere(Point3(0),1.5));

  scene.set_suggested_ticks(1);

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
  scene.addAmbientLight(new AmbientLight(Color(1), 1.6));

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

  ///scene.addShape( new Sphere(Point3(0), .2, Color(0,1,1)));
  ///ImplicitShape *to3 = new Torus(.7, 0.1);
  ///to3->setAlbedo(Color(.3,.3,1));
  /////to3->translate(0,0.2,0);
  ///to3->rotateX(45);
  ///to3->rotateY(30);
  ///scene.addShape(to3);

  ///ImplicitShape *to4 = new Torus(.4, 0.11);
  ///to4->setAlbedo(Color(.7,.3,.3));
  /////to4->translate(0,0.2,0);
  ///to4->rotateX(10);
  ///to4->rotateY(10);
  ///scene.addShape(to4);


  ImplicitShape* sp1 = new Sphere(Point3(-1), 0.99); // 0
  sp1->setAlbedo(Color(.3,0,0)); // rosso a destra dietro blu
  sp1->setShininess(40);
  sp1->setSpecular(Color(0.1));
  //scene.addShape(sp1);
  ImplicitShape* sp2 = new Sphere(Point3(1,-1,1), 0.99); // 4
  sp2->setAlbedo(Color(0,.3,0)); // verde a sinistra da solo
  sp2->setShininess(40);
  sp2->setSpecular(Color(0.1));
  //scene.addShape(sp2);
  ImplicitShape* sp3 = new Sphere(Point3(-1,-1,1), 0.99);
  sp3->setAlbedo(Color(0,0,.3)); // blu a destra davanti a rosso
  sp3->setShininess(40);
  sp3->setSpecular(Color(0.1));
  //scene.addShape(sp3);
  ImplicitShape* sp4 = new Sphere(Point3(0,2,0), 0.99);
  sp4->setAlbedo(Color(0,.3,.3)); // azzurrino in alto
  sp4->setShininess(40);
  sp4->setSpecular(Color(0.1));
  //scene.addShape(sp4);
  ImplicitShape* sp5 = new Sphere(Point3(0,-2,0), 0.99);
  sp5->setAlbedo(Color(.3,.3,0)); // giallo in basso
  sp5->setShininess(40);
  sp5->setSpecular(Color(0.1));
  //scene.addShape(sp5);


  ImplicitShape *csg =
    new UnionShape(
        sp1,
        new UnionShape(
          sp2,
          new UnionShape(
            sp3,
            new UnionShape(
              sp4,
              sp5
              )
            )
          )
        );
  scene.addShape(csg);


  scene.set_suggested_ticks(1);

  // Camera
  //Point3 camera_origin(0,0,0);
  ////Point3 view(0, 0, -1);
  //Vec3 view(0,0,1);
  //float fov = 45;

  Camera cam;
  //int k=2;
  //Point3 cam_pos(k,k,0);
  Point3 cam_pos(0,0,6);
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
  ImplicitShape *sp = new Sphere(.25); // not added to scene // works as general 3D obj
                                       // need to be added  to scene for spin to work
  sp -> setAlbedo(Color(.7,.4,.7));
  scene.addShape(sp);
  float r = .5; // radius
  float t = .1; // thickness
  ImplicitShape *to1 = (new Torus(r,t));
  to1 -> setAlbedo(Color(.9,.4,.4));
  to1 -> translate(-1.f/2.f*r,0,0);
  //to1 -> rotateX(10);
  //to1 -> rotateY(10);
  //to1 -> rotateZ(10);
  to1->setParent(sp);
  scene.addShape(to1);

  ImplicitShape *to2 = (new Torus(r,t));
  to2 -> setAlbedo(Color(.4,.4,.9));
  to2 -> translate(1.f/2.f*r,0,0);
  to2 -> rotateX(90);
  //to2 -> rotateY(10);
  //to2 -> rotateZ(10);
  to2->setParent(sp);
  scene.addShape(to2);

  float rot=45;
  //rot+=15;
  //rot+=15;
  //rot+=15;
  //rot+=15;
  //rot+=15;
  //rot+=15;
  //rot+=15;
  //rot+=15;
  //rot+=15;
  //sp->rotateX(rot);

  sp->set_spin(15,0,0);
  scene.set_suggested_ticks(10);

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

  //float rot=45;
  //rot-=15;
  //rot-=15;
  //rot-=15;
  //csg1->rotateX(rot);
  //csg1->rotateY(rot);

  float rot=0;
  //csg1->rotateX(rot);
  //csg1->rotateY(rot);
  csg1->rotate(20,15,rot);
  csg1->set_spin(0,0,10);

  scene.addShape(csg1);

  //scene.set_suggested_ticks(18);
  scene.set_suggested_ticks(36);
  //scene.set_suggested_ticks(2);

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

Scene makeScene_Smooth() {
  Scene scene;
  // Lights
  scene.addLight(new PointLight(Point3(4,3,2), Color(1), 400));
  scene.addAmbientLight(new AmbientLight(Color(0,.3,0.3), 1));

  float x_offset=1.5;

  // Example 1
  ImplicitShape* sp = new Sphere(Point3(0, 0,0), 1.2);
  ImplicitShape* tol = new Torus (Point3(-x_offset,0,0), 1, 0.3);
  ImplicitShape* tor = new Torus (Point3( x_offset,0,0), 1, 0.3);
  ImplicitShape *csg1 =
    new UnionShape(
        new SmoothUnionShape(sp, tol, 0.5),
        tor
        );
  tol->setAlbedo(Color(.5,.2,.3));
  tor->setAlbedo(Color(.3,.2,.5));

  tol->set_spin(0,0, 10);
  tor->set_spin(0,0,-10);

  ///// Example 2
  //ImplicitShape* to1 = new Torus(Point3(0,0,0), 1, 0.3);
  //ImplicitShape* to2 = new Torus(Point3(0,0,0), 1, 0.3);
  //ImplicitShape *csg1 =
  //  new SmoothUnionShape(
  //      to1,
  //      to2,
  //      0.3f
  //      );
  //to1->rotate(20,10,0);
  //to2->rotateZ(90);
  //to2->set_spin(0,0,15);



  scene.addShape(csg1);
  float rot=20;
  csg1->set_spin(0,20,0);

  scene.set_suggested_ticks((int)(360/rot));


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

Scene makeScene_Mix() {
  Scene scene;
  // Lights
  scene.addLight(new PointLight(Point3(4,3,2), Color(1), 400));
  scene.addAmbientLight(new AmbientLight(Color(0,.3,0.3), 1));


  //ImplicitShape* cu1 = new Cube(Vec3(2,1.5,1.2));
  ImplicitShape* cu1 = new Cube(Vec3(2));
  ImplicitShape* sp1 = new Sphere(2);
  ImplicitShape *csg1 =
    new MixShape(
        cu1,
        sp1,
        0.4f
        );
  scene.addShape(csg1);

  cu1->setAlbedo(Color(.3));
  cu1->setShininess(50);
  cu1->setSpecular(Color(0.1));
  sp1->setAlbedo(Color(.3));
  sp1->setShininess(50);
  sp1->setSpecular(Color(0.1));



  float rot=10;
  cu1->set_spin(rot,rot,0);
  csg1->set_spin(0,rot,0);

  scene.set_suggested_ticks((int)(360/rot));

  // Camera
  Point3 camera_origin(0,3,5);
  float fov = 45;
  Camera cam;
  cam.translate(camera_origin.as_Vec3());
  cam.lookAt(0,0,0);
  scene.addCamera(cam);

  return scene;
}

float f(Point3 p) {
  //return sin(20*p.x())*sin(20*p.y())*sin(20*p.z());
  float intensity=.2;
  float freq=5;
  return intensity *
    std::sin(freq*p.x()) *
    std::sin(freq*p.y()) *
    std::sin(freq*p.z())
    ;
}
Scene makeScene_Operations() {
  Scene scene;
  // Lights
  //scene.addLight(new PointLight(Point3(0,5,0), Color(1), 200));
  scene.addLight(new PointLight(Point3(5,4,2), Color(1), 300));
  scene.addAmbientLight(new AmbientLight(Color(0,.3,0.3), 1.5));


  ImplicitShape* sp1 = new Sphere(.2);
  //ImplicitShape *csg1 =
  //  new DisplacementOpShape(
  //      sp1,
  //      //to1,
  //      &f
  //      );
  ImplicitShape *csg1 =
    new RepeteOpShape(
        sp1,
        Vec3(.5,.5,.5),
        Vec3(1,2,1)
        );
  scene.addShape(csg1);

  float rot=10;
  //csg1->set_spin(0,rot,0);

  //scene.set_suggested_ticks(1);
  scene.set_suggested_ticks((int)(360/rot));

  // Camera
  Point3 camera_origin(0,3,5);
  float fov = 45;
  Camera cam;
  cam.translate(camera_origin.as_Vec3());
  cam.lookAt(0,0,0);
  scene.addCamera(cam);

  return scene;
}

Scene makeScene_Octree() {
  Scene scene;
  // Lights
  scene.addLight(new PointLight(Point3(5, 4,0), Color(1), 400));
  //scene.addLight(new PointLight(Point3(0,-5,0), Color(1), 400));
  scene.addAmbientLight(new AmbientLight(Color(1), 1.0));


  ///// Fully inside first lvl children of cube 4x4x4
  //// 0
  //ImplicitShape* sp1 = new Sphere(Point3(-1), 0.99);
  //sp1->setAlbedo(Color(.3,0,0));
  //sp1->setShininess(40);
  //sp1->setSpecular(Color(0.1));
  //scene.addShape(sp1);
  //// 7
  //ImplicitShape* sp2 = new Sphere(Point3(1), 0.99);
  //sp2->setAlbedo(Color(0,.3,0));
  //sp2->setShininess(40);
  //sp2->setSpecular(Color(0.1));
  //scene.addShape(sp2);
  //// 1
  //ImplicitShape* sp3 = new Sphere(Point3(-1,-1,1), 0.99);
  //sp3->setAlbedo(Color(0,0,.3));
  //sp3->setShininess(40);
  //sp3->setSpecular(Color(0.1));
  //scene.addShape(sp3);

  ///// Centered touching 1 child of each lvl 1 root's children (octree height = 3)
  //ImplicitShape* sp1 = new Sphere(Point3(0), 0.99);
  //sp1->setAlbedo(Color(.3,0,0));
  //sp1->setShininess(40);
  //sp1->setSpecular(Color(0.1));
  //scene.addShape(sp1);

  ///// Touching 2 child of each of 4 first-lvl voxels on the right (octree dim=2^2, height=3)
  //ImplicitShape* sp1 = new Sphere(Point3(1,0,0), 0.99);
  //sp1->setAlbedo(Color(.3,0,0));
  //sp1->setShininess(40);
  //sp1->setSpecular(Color(0.1));
  //scene.addShape(sp1);


  /// Colors associated with children names
  float root_cube_dim = 4.f;
  float d = root_cube_dim/2.f - root_cube_dim/4.f; // sphere child-centered centered 
  float sp_rad = root_cube_dim/4.f - root_cube_dim/50.f; // half of half of child dim (minus a bit to not touch child edges)
  float c = 0.8f;  // color
  float n = 0.09f; // no color

  std::cout <<
    "\nroot_cube_dim = " << root_cube_dim <<
    "\nd = " << d <<
    "\nsp_rad = " << sp_rad <<
    std::endl;

  // (-d,-d,-d) ~ 000 ~ 0
  ImplicitShape* c_000 = new Sphere(Point3(-d,-d,-d), sp_rad);
  c_000->setAlbedo(Color(n,n,n));
  scene.addShape(c_000);

  // (-d,-d, d) ~ 001 ~ 1
  ImplicitShape* c_001 = new Sphere(Point3(-d,-d, d), sp_rad);
  c_001->setAlbedo(Color(n,n,c));
  scene.addShape(c_001);

  // ( d,-d, d) ~ 010 ~ 2
  ImplicitShape* c_010 = new Sphere(Point3(-d, d,-d), sp_rad);
  c_010->setAlbedo(Color(n,c,n));
  scene.addShape(c_010);

  // (-d, d, d) ~ 011 ~ 3
  ImplicitShape* c_011 = new Sphere(Point3(-d, d, d), sp_rad);
  c_011->setAlbedo(Color(n,c,c));
  scene.addShape(c_011);

  // (d,-d,-d) ~ 100 ~ 4
  ImplicitShape* c_100 = new Sphere(Point3( d,-d,-d), sp_rad);
  c_100->setAlbedo(Color(c,n,n));
  scene.addShape(c_100);

  // ( d,-d, d) ~ 101 ~ 5
  ImplicitShape* c_101 = new Sphere(Point3( d,-d, d), sp_rad);
  c_101->setAlbedo(Color(c,n,c));
  scene.addShape(c_101);

  // (d, d,-d) ~ 110 ~ 6
  ImplicitShape* c_110 = new Sphere(Point3( d, d,-d), sp_rad);
  c_110->setAlbedo(Color(c,c,n));
  scene.addShape(c_110);

  // ( d, d, d) ~ 111 ~ 7
  ImplicitShape* c_111 = new Sphere(Point3( d, d, d), sp_rad);
  c_111->setAlbedo(Color(c,c,c));
  scene.addShape(c_111);


  scene.set_suggested_ticks(1);


  // Camera
  //Point3 camera_origin(5* Vec3(-1.5,2,1).normalize().as_Point());
  //Point3 camera_origin(5* Vec3(0,0,1).normalize().as_Point());
  //Point3 camera_origin(Vec3(0,0,5).as_Point());
  //Point3 camera_origin(0,0,5);
  //Point3 camera_origin(0.00000001,0.00000001,5);
  Point3 camera_origin(0.00001,0.00001,7); // For now only this is stable and useful
  //Point3 camera_origin(-0.00001,-0.00001,7); // TODO use this to test camera-octree positioning
  //Point3 camera_origin(1,1,7);
  float fov = 45;
  Camera cam;
  cam.translate(camera_origin.as_Vec3());
  cam.lookAt(0,0,0);
  scene.addCamera(cam);

  return scene;
}

Scene makeScene_Octree_1() {
  Scene scene;
  // Lights
  scene.addLight(new PointLight(Point3(5, 4,0), Color(1), 400));
  //scene.addLight(new PointLight(Point3(0,-5,0), Color(1), 400));
  scene.addAmbientLight(new AmbientLight(Color(1), 1.0));


  /// Colors associated with children names
  float root_cube_dim = 8.f;
  float d = root_cube_dim/2.f - root_cube_dim/4.f; // sphere child-centered centered 
  float sp_rad = root_cube_dim/4.f - root_cube_dim/50.f; // half of half of child dim (minus a bit to not touch child edges)
  float c = 0.90f;  // color
  float n = 0.30f; // no color

  std::cout <<
    "\nroot_cube_dim = " << root_cube_dim <<
    "\nd = " << d <<
    "\nsp_rad = " << sp_rad <<
    std::endl;

  //// (-d,-d,-d) ~ 000 ~ 0
  //ImplicitShape* c_000 = new Sphere(Point3(-d,-d,-d), sp_rad);
  //c_000->setAlbedo(Color(n,n,n));
  //scene.addShape(c_000);
  { // 000 has all children
    Point3 p_pos(-d,-d,-d); // parent position offset
    Color  p_col(n,n,n);    // parent color offset
    float o_sp_rad = sp_rad;
    float d = root_cube_dim/4 - root_cube_dim/8;
    float sp_rad = o_sp_rad/4;
    // (-d,-d,-d) ~ 000 ~ 0
    ImplicitShape* c_000 = new Sphere(p_pos + Point3(-d,-d,-d), sp_rad);
    c_000->setAlbedo(p_col * Color(n,n,n));
    scene.addShape(c_000);
    // (-d,-d, d) ~ 001 ~ 1
    ImplicitShape* c_001 = new Sphere(p_pos + Point3(-d,-d, d), sp_rad);
    c_001->setAlbedo(p_col * Color(n,n,c));
    scene.addShape(c_001);
    // ( d,-d, d) ~ 010 ~ 2
    ImplicitShape* c_010 = new Sphere(p_pos + Point3(-d, d,-d), sp_rad);
    c_010->setAlbedo(p_col * Color(n,c,n));
    scene.addShape(c_010);
    // (-d, d, d) ~ 011 ~ 3
    ImplicitShape* c_011 = new Sphere(p_pos + Point3(-d, d, d), sp_rad);
    c_011->setAlbedo(p_col * Color(n,c,c));
    scene.addShape(c_011);
    // (d,-d,-d) ~ 100 ~ 4
    ImplicitShape* c_100 = new Sphere(p_pos + Point3( d,-d,-d), sp_rad);
    c_100->setAlbedo(p_col * Color(c,n,n));
    scene.addShape(c_100);
    // ( d,-d, d) ~ 101 ~ 5
    ImplicitShape* c_101 = new Sphere(p_pos + Point3( d,-d, d), sp_rad);
    c_101->setAlbedo(p_col * Color(c,n,c));
    scene.addShape(c_101);
    // (d, d,-d) ~ 110 ~ 6
    ImplicitShape* c_110 = new Sphere(p_pos + Point3( d, d,-d), sp_rad);
    c_110->setAlbedo(p_col * Color(c,c,n));
    scene.addShape(c_110);
    // ( d, d, d) ~ 111 ~ 7
    ImplicitShape* c_111 = new Sphere(p_pos + Point3( d, d, d), sp_rad);
    c_111->setAlbedo(p_col * Color(c,c,c));
    scene.addShape(c_111);
  } // END 000 children

  //// (-d, d, d) ~ 011 ~ 3
  //ImplicitShape* c_011 = new Sphere(Point3(-d, d, d), sp_rad);
  //c_011->setAlbedo(Color(n,c,c));
  //scene.addShape(c_011);
  { // 011 has children num 0,3,5
    Point3 p_pos(-d, d, d); // parent position offset
    Color  p_col(n,c,c);    // parent color offset
    float o_sp_rad = sp_rad;
    float d = root_cube_dim/4 - root_cube_dim/8;
    float sp_rad = o_sp_rad/4;
    // (-d,-d,-d) ~ 000 ~ 0
    ImplicitShape* c_000 = new Sphere(p_pos + Point3(-d,-d,-d), sp_rad);
    c_000->setAlbedo(p_col * Color(n,n,n));
    scene.addShape(c_000);
    // (-d, d, d) ~ 011 ~ 3
    ImplicitShape* c_011 = new Sphere(p_pos + Point3(-d, d, d), sp_rad);
    c_011->setAlbedo(p_col * Color(n,c,c));
    scene.addShape(c_011);
    // ( d,-d, d) ~ 101 ~ 5
    ImplicitShape* c_101 = new Sphere(p_pos + Point3( d,-d, d), sp_rad);
    c_101->setAlbedo(p_col * Color(c,n,c));
    scene.addShape(c_101);
  } // END 011 children

  //// ( d,-d, d) ~ 101 ~ 5
  //ImplicitShape* c_101 = new Sphere(Point3( d,-d, d), sp_rad);
  //c_101->setAlbedo(Color(c,n,c));
  //scene.addShape(c_101);
  { // 101 has all children
    Point3 p_pos( d,-d, d); // parent position offset
    Color  p_col(c,n,c);    // parent color offset
    float o_sp_rad = sp_rad;
    float d = root_cube_dim/4 - root_cube_dim/8;
    float sp_rad = o_sp_rad/4;
    // (-d,-d, d) ~ 001 ~ 1
    ImplicitShape* c_001 = new Sphere(p_pos + Point3(-d,-d, d), sp_rad);
    c_001->setAlbedo(p_col * Color(n,n,c));
    scene.addShape(c_001);
    // ( d,-d, d) ~ 010 ~ 2
    ImplicitShape* c_010 = new Sphere(p_pos + Point3(-d, d,-d), sp_rad);
    c_010->setAlbedo(p_col * Color(n,c,n));
    scene.addShape(c_010);
    // (d, d,-d) ~ 110 ~ 6
    ImplicitShape* c_110 = new Sphere(p_pos + Point3( d, d,-d), sp_rad);
    c_110->setAlbedo(p_col * Color(c,c,n));
    scene.addShape(c_110);
  } // END 101 children

  scene.set_suggested_ticks(1);

  // Camera
  float cam_distance = root_cube_dim + 1;
  Point3 camera_origin(cam_distance* Vec3(0.7,0.7,0.5).normalize().as_Point());
  //Point3 camera_origin(cam_distance* Vec3(0,0,1).normalize().as_Point());
  float fov = 45;
  Camera cam;
  cam.translate(camera_origin.as_Vec3());
  cam.lookAt(0,0,0);
  scene.addCamera(cam);

  return scene;
}


Scene makeScene_Octree_2() {
  Scene scene;
  // Lights
  scene.addLight(new PointLight(Point3(5, 4,0), Color(1), 400));
  //scene.addLight(new PointLight(Point3(0,-5,0), Color(1), 400));
  scene.addAmbientLight(new AmbientLight(Color(1), 1.0));


  /// Colors associated with children names
  float root_cube_dim = 8.f;
  float d = root_cube_dim/2.f - root_cube_dim/4.f; // sphere child-centered centered 
  float sp_rad = root_cube_dim/4.f - root_cube_dim/50.f; // half of half of child dim (minus a bit to not touch child edges)
  float c = 0.90f;  // color
  float n = 0.30f; // no color

  std::cout <<
    "\nroot_cube_dim = " << root_cube_dim <<
    "\nd = " << d <<
    "\nsp_rad = " << sp_rad <<
    std::endl;

  //// (-d,-d, d) ~ 001 ~ 1
  //ImplicitShape* c_001 = new Sphere(Point3(-d,-d, d), sp_rad);
  //c_001->setAlbedo(Color(n,n,c));
  //scene.addShape(c_001);
  
  
  // small sphere completely inside child 1->7->2
  d = root_cube_dim/(2*2*2);
  ImplicitShape* c_001_111_010 = new Sphere(Point3(d,3*d,d), d/1.5);
  c_001_111_010->setAlbedo(Color(c,c,c));
  scene.addShape(c_001_111_010);

  scene.set_suggested_ticks(1);

  // Camera
  float cam_distance = root_cube_dim + 1;
  //Point3 camera_origin(.2,.2,7);
  //Point3 camera_origin(0.00001,0.00001,7); // For now only this is stable and useful
  //Point3 camera_origin(1,1,7);

  float fov = 45;
  Camera cam;
  d = root_cube_dim/2.f+1;
  Point3 camera_origin(d,d,2*d);
  cam.translate(camera_origin.as_Vec3());
  cam.lookAt(0,0,0);
  scene.addCamera(cam);

  return scene;
}

