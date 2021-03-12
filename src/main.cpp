#include <iostream>

#include "Image.h"
#include "Camera.h"
#include "Window.h"
#include "Renderer.h"

int main() {
  // Image
  const float aspect_ratio = 16.0/9.0;
  const int width  = 400;

  // Window
  Window window(width, aspect_ratio);

  // Camera
  Point3 camera_origin(0, 4, 5);
  Vec3 camera_dir = Vec3(0, 0.5, -1);
  camera_dir.normalize();
  float fov = 45;
  Camera cam(fov, camera_origin, camera_dir);

  Renderer renderer(window, cam);
  renderer.render();

  return 0;
}
