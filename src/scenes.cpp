#include "scenes.h"
#include <cmath>
#include <iostream>

Scene makeScene_Octree() {
  std::cout << "Scene: makeScene_Octree" << std::endl;
  Scene scene;
  // Lights
  scene.addLight(new PointLight(Point3(5, 4,0), Color(1), 400));
  //scene.addLight(new PointLight(Point3(0,-5,0), Color(1), 400));
  scene.addAmbientLight(new AmbientLight(Color(1), 1.0));

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
  float fov = 45;
  Point3 camera_origin(5,5,10);

  Camera cam;
  cam.translate(camera_origin.as_Vec3());
  cam.lookAt(0,0,0);
  scene.addCamera(cam);

  return scene;
}

Scene makeScene_Octree_1() {
  std::cout << "Scene: makeScene_Octree_1" << std::endl;
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
  Point3 camera_origin(5,5,10);
  float fov = 45;
  Camera cam;
  cam.translate(camera_origin.as_Vec3());
  cam.lookAt(0,0,0);
  scene.addCamera(cam);

  return scene;
}


Scene makeScene_Octree_2() {
  std::cout << "Scene: makeScene_Octree_2" << std::endl;
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
  ImplicitShape* c_001_111_010 = new Sphere(
      Point3(
        1,
        3,
        1),
      0.5);
  c_001_111_010->setAlbedo(Color(c,c,c));
  scene.addShape(c_001_111_010);

  scene.set_suggested_ticks(1);

  // Camera
  float fov = 45;
  Camera cam;
  Point3 camera_origin(5,5,10);
  cam.translate(camera_origin.as_Vec3());
  cam.lookAt(0,0,0);
  scene.addCamera(cam);

  return scene;
}

Scene makeScene_Octree_3() {
  std::cout << "Scene: makeScene_Octree_3" << std::endl;
  Scene scene;
  // Lights
  scene.addLight(new PointLight(Point3(5, 4,0), Color(1), 400));
  //scene.addLight(new PointLight(Point3(0,-5,0), Color(1), 400));
  scene.addAmbientLight(new AmbientLight(Color(1), 1.0));


  /// Colors associated with children names
  float root_cube_dim = 8.f;
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
  float fov = 45;
  float cam_distance = root_cube_dim + 1;

  Point3 camera_origin(5,5,10);
  Camera cam;
  cam.translate(camera_origin.as_Vec3());
  cam.lookAt(0,0,0);
  scene.addCamera(cam);

  return scene;
}

Scene makeScene_Octree_4() {
  std::cout << "Scene: makeScene_Octree_4" << std::endl;
  Scene scene;
  // Lights
  scene.addLight(new PointLight(Point3(5, 4,0), Color(1), 400));
  scene.addAmbientLight(new AmbientLight(Color(1), 1.0));

  /// Colors associated with children names
  float root_cube_dim = 8.f;
  float d = root_cube_dim/2.f - root_cube_dim/4.f; // sphere child-centered centered 
  float sp_rad = root_cube_dim/4.f - root_cube_dim/50.f; // half of half of child dim (minus a bit to not touch child edges)
  float c = 0.8f;  // color
  float n = 0.09f; // no color

  // Shapes
  scene.addShape(new Torus(Point3(0,3,0),1,0.2));
  scene.addShape(new Torus(Point3(0,1.5,0),1.5,0.4));
  scene.addShape(new Torus(2,0.65));

  //scene.addShape(new Sphere(Point3(0,-102,0),100)); // Terrain

  scene.set_suggested_ticks(1);

  // Camera
  float fov = 45;
  float cam_distance = root_cube_dim + 1;

  Point3 camera_origin(5,5,10);
  Camera cam;
  cam.translate(camera_origin.as_Vec3());
  cam.lookAt(0,0,0);
  scene.addCamera(cam);

  return scene;
}

