/**
 * @file      renderer.cpp
 * @author    Rei Shimizu (shikugawa) <shikugawa@gmail.com>
 * @brief     Low level SDL2 wrapper of renderer
 *
 * @copyright Copyright 2021 Rei Shimizu. All rights reserved.
 */

#include "renderer.h"

#include <absl/strings/str_format.h>

#include "common/exception.h"

namespace Truffle {

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