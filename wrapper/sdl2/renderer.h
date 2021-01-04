/**
 * @file      renderer.h
 * @author    Rei Shimizu (shikugawa) <shikugawa@gmail.com>
 * @brief     Low level SDL2 wrapper of renderer
 *
 * @copyright Copyright 2021 Rei Shimizu. All rights reserved.
 */

#ifndef TRUFFLE_RENDERER_H
#define TRUFFLE_RENDERER_H

#include <SDL2/SDL.h>

#include "color.h"
#include "common/non_copyable.h"
#include "common/singleton.h"
#include "window.h"

namespace Truffle {

class Renderer : public MutableSingleton<Renderer>, NonCopyable {
 public:
  ~Renderer();

  void setDrawColor(const Color& color);
  void setDrawColor(Color&& color);

  [[nodiscard]] SDL_Renderer const* entity() const& { return renderer_entity_; }

 private:
  friend class MutableSingleton<Renderer>;

  explicit Renderer(const Window& window);

  SDL_Renderer* renderer_entity_;
};

}  // namespace Truffle
#endif  // TRUFFLE_RENDERER_H
