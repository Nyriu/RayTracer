#ifndef WINDOW_H
#define WINDOW_H

#include <iostream>
#include <cmath>

#include <GLFW/glfw3.h>

#include "Image.h"

static void error_callback(int error, const char* description);

class Window {
  public:
    const float aspect_ratio;
    const int width, height;
  private:
    GLFWwindow* window_ = nullptr;

    bool is_window_open = false;

  public:
    Window(const int win_width, const int win_height) :
      width(win_width), height(win_height), aspect_ratio((float)win_width/(float)win_height) {
        glfwSetErrorCallback(error_callback);
        if (!glfwInit()) {
          perror("Couldn't init GLFW\n");
          exit(1);
        }
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
      }

    void openWindow();
    void quitWindow();
    bool isOpen() const { return is_window_open; }

    void drawImage(const Image& image);
    bool keepRendering();
};

static void error_callback(int error, const char* description) {
  fprintf(stderr, "Error: %s\n", description);
}


#endif
