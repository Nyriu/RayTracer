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

  //const int width = 1080;
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
    //makeScene_CameraTransformDebug();
    //makeScene_HierarchyTransformDebug();
    makeScene_HierarchyCSGTransformDebug();


  Renderer renderer(&window);
  renderer.disableWindow();
  //renderer.enableWindow();

  renderer.setScene(&scene, true); // true to generate scene's right number of frames
  //renderer.addCamera(Camera ... );
  renderer.render();

  return 0;
}
