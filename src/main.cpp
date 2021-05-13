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
  //const int height = 720;
  //const int width = height*16/9;

  const int height = 240;
  const int width = height*4/3;

  // Window
  Window window(width, height);

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
    makeScene_HierarchyTransformDebug();
    //makeScene_HierarchyCSGTransformDebug();


  Renderer renderer(&window);
  //renderer.disableWindow();
  renderer.enableWindow();

  renderer.setScene(&scene, true); // true to generate scene's right number of frames
  //renderer.addCamera(Camera ... );
  renderer.render();

  return 0;
}
