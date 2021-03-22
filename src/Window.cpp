#include "Window.h"


void Window::openWindow() {
  if (!is_window_open) {
    SDL_CreateWindowAndRenderer(width, height, 0, &sdl_window, &sdl_renderer);
    SDL_RenderClear(sdl_renderer);
    is_window_open = true;
  }
}

void Window::quitWindow() {
  if (is_window_open) {
    SDL_DestroyRenderer(sdl_renderer);
    SDL_DestroyWindow(sdl_window);
    SDL_Quit();
    is_window_open = false;
  }
}

void Window::drawImage(const Image& image) {
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

bool Window::keepRendering() {
  SDL_Delay(16);
  if (SDL_PollEvent(&sdl_event) && sdl_event.type == SDL_QUIT) {
    quitWindow();
    return false;
  }
  return true;
}
