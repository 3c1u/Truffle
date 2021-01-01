//
// Created by shikugawa on 2020/12/19.
//

#ifndef TRUFFLE_RENDERER_H
#define TRUFFLE_RENDERER_H

#include <SDL2/SDL.h>

#include "color.h"
#include "window.h"

namespace Truffle {

class Renderer : public MutableSingleton<Renderer>, NonCopyable {
 public:
  void setDrawColor(const Color& color);
  void setDrawColor(Color&& color);

  [[nodiscard]] SDL_Renderer const* entity() const& { return renderer_entity_; }

 private:
  friend class MutableSingleton<Renderer>;

  explicit Renderer(const Window& window);
  ~Renderer();

  SDL_Renderer* renderer_entity_;
};

Renderer::Renderer(const Window& window) {
  renderer_entity_ =
      SDL_CreateRenderer(const_cast<SDL_Window*>(window.entity()), -1,
                         SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (!renderer_entity_) {
    throw TruffleException(absl::StrFormat(
        "Failed to create renderer, bound to window %s", window.name()));
  }
}

Renderer::~Renderer() { SDL_DestroyRenderer(renderer_entity_); }

void Renderer::setDrawColor(const Color& c) {
  SDL_SetRenderDrawColor(renderer_entity_, c.r, c.g, c.b, c.a);
}

void Renderer::setDrawColor(Color&& c) {
  SDL_SetRenderDrawColor(renderer_entity_, c.r, c.g, c.b, c.a);
}

}  // namespace Truffle
#endif  // Truffle_RENDERER_H
