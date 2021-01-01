//
// Created by shikugawa on 2020/12/18.
//

#ifndef TRUFFLE_WINDOW_H
#define TRUFFLE_WINDOW_H

#include <SDL2/SDL.h>
#include <absl/strings/str_format.h>

#include <memory>
#include <string>

#include "exception.h"
#include "non_copyable.h"
#include "singleton.h"

namespace Truffle {

class Window : public ConstSingleton<Window>, NonCopyable {
 public:
  [[nodiscard]] const std::string& name() const& { return name_; }
  [[nodiscard]] SDL_Window const* entity() const& { return window_entity_; }

 private:
  friend class ConstSingleton<Window>;

  Window(std::string name, int width, int height);
  ~Window();

  SDL_Window* window_entity_;
  const std::string name_;
  const int width_;
  const int height_;
};

Window::Window(std::string name, int width, int height)
    : name_(name), width_(width), height_(height) {
  window_entity_ = SDL_CreateWindow(name_.c_str(), SDL_WINDOWPOS_UNDEFINED,
                                    SDL_WINDOWPOS_UNDEFINED, width_, height_,
                                    SDL_WINDOW_SHOWN);
  if (!window_entity_) {
    throw TruffleException(
        absl::StrFormat("Failed to create %s window", name_));
  }
}

Window::~Window() { SDL_DestroyWindow(window_entity_); }

}  // namespace Truffle

#endif  // Truffle_WINDOW_H
