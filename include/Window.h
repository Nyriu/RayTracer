#ifndef WINDOW_H
#define WINDOW_H

#include <cmath>

#include <SDL2/SDL.h>
#include "Image.h"

class Window {
  public:
    const float aspect_ratio;
    const int width, height;
  private:
    SDL_Event sdl_event;
    SDL_Renderer *sdl_renderer;
    SDL_Window *sdl_window;

    bool is_window_open = false;

  public:
    Window(const int win_width, const float aspect_ratio) :
      width(win_width), aspect_ratio(aspect_ratio), height((int)(width / aspect_ratio)) {
        SDL_Init(SDL_INIT_VIDEO);
      }

    void openWindow();
    void quitWindow();
    bool isOpen() const { return is_window_open; }

    void drawImage(const Image& image);
    bool keepRendering();
};


#endif
