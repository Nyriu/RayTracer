#include <cmath>
#include <iostream>

#include "Image.h"
#include "Camera.h"
#include "Window.h"
#include "Renderer.h"
#include "Octree.h"
#include "scenes.h"

#include "geometry.h"

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
  //const int height = 720;
  //const int width = height*16/9;

  //const int height = 480;
  //const int width = height*4/3;

  const int height = 240;
  const int width = height*4/3;

  //const int height = 8;
  //const int width  = 8;

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
    //makeScene_Octree_1();
    //makeScene_Octree_2();
    makeScene_Octree_3();

  default_rendering(window, scene);
  octree_rendering(window, scene);

  return 0;
}
