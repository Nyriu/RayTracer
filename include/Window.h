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

    void openWindow() {
      if (!is_window_open) {
        SDL_CreateWindowAndRenderer(width, height, 0, &sdl_window, &sdl_renderer);
        SDL_RenderClear(sdl_renderer);
        is_window_open = true;
      }
    }

    void quitWindow() {
      if (is_window_open) {
        SDL_DestroyRenderer(sdl_renderer);
        SDL_DestroyWindow(sdl_window);
        SDL_Quit();
        is_window_open = false;
      }
    }

    bool isOpen() const { return is_window_open; }

    void drawImage(const Image& image) {
      // TODO check if image inside window
      for (int j=0; j<height; ++j) {
        for (int i=0; i<width; ++i) {
          Color pc = image.getPixel(i,j);
          SDL_SetRenderDrawColor(sdl_renderer,
              pc.r255(), pc.g255(), pc.b255(),
              255);
          SDL_RenderDrawPoint(sdl_renderer, i, j);
        }
      }
      SDL_RenderPresent(sdl_renderer);
    }

    bool keepRendering() {
    	SDL_Delay(16);
      if (SDL_PollEvent(&sdl_event) && sdl_event.type == SDL_QUIT) {
        quitWindow();
        return false;
      }
      return true;
    }



};


#endif
