#include <iostream>

#include "Image.h"
#include "Renderer.h"

int main() {

  // Image
  const float aspect_ratio = 16.0/9.0;
  const int image_width  = 400;

  Renderer renderer(image_width, aspect_ratio);
  renderer.render();


  return 0;
}
