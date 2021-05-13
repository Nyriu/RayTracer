#include "Window.h"
#include <iostream>


static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}


void Window::openWindow() {
  if (!is_window_open) {
    window_ = glfwCreateWindow(width, height, "GLFW Window", NULL, NULL);
    if (!window_ || window_ == nullptr) {
      perror("Couldn't open window\n");
      exit(1);
    }

    glfwSetKeyCallback(window_, key_callback);
    glfwMakeContextCurrent(window_);
    glfwSwapInterval(1);

    is_window_open = true;
  }
}

void Window::quitWindow() {
  if (is_window_open) {
    glfwDestroyWindow(window_);
    glfwTerminate();

    is_window_open = false;
  }
}


void Window::drawImage(const Image& image) {
  int img_width = image.width;
  int img_height = image.height;
  unsigned char* data = new unsigned char[img_width * img_height * 3];
  for (int y = 0; y < img_height; y++) {
    for (int x = 0; x < img_width; x++) {
      Color pc = image.getPixel(x,height-y);
      data[3*(y*img_width + x) + 0] = pc.r255();
      data[3*(y*img_width + x) + 1] = pc.g255();
      data[3*(y*img_width + x) + 2] = pc.b255();
    }
  }

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glDrawPixels(img_width, img_height, GL_RGB, GL_UNSIGNED_BYTE, data);

  glfwSwapBuffers(window_);
  glfwPollEvents();
}

bool Window::keepRendering() {
  return !glfwWindowShouldClose(window_);
}
