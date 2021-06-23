#include <cmath>
#include <iostream>

#include "Image.h"
#include "Camera.h"
#include "Window.h"
#include "Renderer.h"
#include "Octree.h"
#include "scenes.h"
#include "geometry.h"

#include <chrono>
#include <thread>

void default_rendering(Window& window, Scene& scene) {
  Renderer renderer(&window);
  renderer.disableWindow();
  //renderer.enableWindow();

  renderer.setScene(&scene, true); // true to generate scene's right number of frames
  //renderer.addCamera(Camera ... );
  renderer.render();
}

void octree_rendering(Window& window, Scene& scene) {
  //Octree oct;
  //oct.fromScene(&scene);

  Renderer renderer(&window);
  renderer
    .disableWindow();
    //.enableWindow();

  renderer
    .setScene(&scene)
    .useOctree()
    .setCamera(scene.getCamera())
    .render();
}


int main() {
  // Image
  const int height = 1080;
  const int width = height*16/9;

  //const int height = 720;
  //const int width = height*16/9;

  //const int height = 480;
  //const int width = height*4/3;

  //const int height = 240;
  //const int width = height*4/3;

  std::cout << "Image " << width << "x" << height << std::endl;

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
    //makeScene_HierarchyTransformDebug();
    //makeScene_HierarchyCSGTransformDebug();
    //makeScene_Smooth();
    //makeScene_Mix();
    //makeScene_Operations();
    //makeScene_Octree();
    makeScene_Octree_1();
    //makeScene_Octree_2();
    //makeScene_Octree_3();
    //makeScene_Octree_4();


  std::cout << "\nRendering without octree (default)" << std::endl;
  default_rendering(window, scene);
  using namespace std::this_thread; // sleep_for, sleep_until
  sleep_for(std::chrono::nanoseconds(100));

  std::cout << "\n\nRendering with octree" << std::endl;
  octree_rendering(window, scene);

  return 0;
}
