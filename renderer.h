//
// Created by shikugawa on 2020/12/19.
//

#ifndef TRUFFLE_RENDERER_H
#define TRUFFLE_RENDERER_H

#include <SDL2/SDL.h>

#include "window.h"

namespace Truffle {

class Renderer : NonCopyable {
 public:
  ~Renderer() { SDL_DestroyRenderer(renderer_entity_); }

  void init(Window& window) {
    if (initialized_) {
      return;
    }

    if (!window.initialized()) {
      throw TruffleException(
          absl::StrFormat("Window %s doesn't initialized.", window.name()));
    }
    renderer_entity_ = SDL_CreateRenderer(
        const_cast<SDL_Window*>(window.windowEntity()), -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer_entity_) {
      throw TruffleException(absl::StrFormat(
          "Failed to create renderer, bound to window %s", window.name()));
    }

    initialized_ = true;
  }

  void setDrawColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    if (!initialized_) {
      return;
    }
    SDL_SetRenderDrawColor(renderer_entity_, r, g, b, a);
  }

  void render() {
    if (!initialized_) {
      return;
    }
    SDL_RenderClear(renderer_entity_);
    SDL_RenderPresent(renderer_entity_);
  }

  SDL_Renderer* entity() { return renderer_entity_; }

 private:
  bool initialized_{false};
  SDL_Renderer* renderer_entity_;
};

}  // namespace Truffle
#endif  // Truffle_RENDERER_H
