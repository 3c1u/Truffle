/**
 * @file      window.h
 * @author    Rei Shimizu (shikugawa) <shikugawa@gmail.com>
 * @brief     Low level SDL2 wrapper of window
 *
 * @copyright Copyright 2021 Rei Shimizu. All rights reserved.
 */

#ifndef TRUFFLE_WINDOW_H
#define TRUFFLE_WINDOW_H

#include <SDL2/SDL.h>

#include <string>

#include "common/non_copyable.h"
#include "common/singleton.h"

namespace Truffle {

class Window : public ConstSingleton<Window>, NonCopyable {
 public:
  ~Window();

  [[nodiscard]] const std::string& name() const& { return name_; }
  [[nodiscard]] SDL_Window const* entity() const& { return window_entity_; }

 private:
  friend class ConstSingleton<Window>;

  Window(std::string name, int width, int height);

  SDL_Window* window_entity_;
  const std::string name_;
  const int width_;
  const int height_;
};

}  // namespace Truffle

#endif  // TRUFFLE_WINDOW_H
