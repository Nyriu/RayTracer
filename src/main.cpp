#include <cmath>
#include <iostream>

#include "Image.h"
#include "Camera.h"
#include "Window.h"
#include "Renderer.h"
#include "scenes.h"

int main() {
  // Image
  //const float aspect_ratio = 16.0/9.0;
  const float aspect_ratio = 1;

  const int width = 300;
  //const int width = 200;
  //const int width = 100;
  //const int width = 400 * 2;

  // Window
  Window window(width, aspect_ratio);

  // Scene
  auto scene =
    makeScene_1();
    //makeScene_Spheres();
    //makeScene_Toruses();


  Renderer renderer(&window);
  renderer.disableWindow();
  //renderer.enableWindow();

  renderer.setScene(&scene);
  //renderer.addCamera(Camera ... );
  renderer.render();

  return 0;
}
