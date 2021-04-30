#include <cmath>
#include <iostream>

#include "Image.h"
#include "Camera.h"
#include "Window.h"
#include "Renderer.h"
#include "scenes.h"

#include "geometry.h"

int main() {
  // Image
  const float aspect_ratio = 16.0/9.0;
  //const float aspect_ratio = 1;

  //const int width = 900;
  const int width = 300;
  //const int width = 200;
  //const int width = 100;
  //const int width = 50;
  //const int width = 26;
  //const int width = 10;
  //const int width = 2;
  //const int width = 400 * 2;

  // Window
  Window window(width, aspect_ratio);

  // Scene
  auto scene =
    //makeScene_1();
    //makeScene_Spheres();
    //makeScene_Toruses();
    //makeScene_CSG_1();
    //makeScene_CSG_2();
    //makeScene_CSG_3();
    //makeScene_ShadowDebug();
    //makeScene_TransformDebug();
    makeScene_CameraTransformDebug();


  Renderer renderer(&window);
  renderer.disableWindow();
  //renderer.enableWindow();

  renderer.setScene(&scene);
  //renderer.addCamera(Camera ... );
  renderer.render();


  // Mat4 m;
  // m.set(0,0,1);
  // m.set(0,1,2);
  // //m.set(3,2,3);
  // m.set_col_0(Vec3(1,2,3));
  // m.set_col_1(Vec3(4,5,6));
  // m.set_col_2(Vec3(7,8,9));
  // m.set_col_3(Vec3(10,11,12));
  // std::cout << "\nm\n" << m << "\n" <<
  //   m.get(0,0) << " " << m.get(0,1) << " " << m.get(0,2) << " "<< m.get(0,3) << "\n" <<
  //   m.get(1,0) << " " << m.get(1,1) << " " << m.get(1,2) << " "<< m.get(1,3) << "\n" <<
  //   m.get(2,0) << " " << m.get(2,1) << " " << m.get(2,2) << " "<< m.get(2,3) << "\n" <<
  //   m.get(3,0) << " " << m.get(3,1) << " " << m.get(3,2) << " "<< m.get(3,3) << "\n" <<
  //   std::endl;



  return 0;
}
