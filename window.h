//
// Created by shikugawa on 2020/12/18.
//

#ifndef MIRAI_WINDOW_H
#define MIRAI_WINDOW_H

#include <SDL2/SDL.h>
#include <absl/strings/str_format.h>

#include <memory>
#include <string>

#include "exception.h"
#include "non_copyable.h"

namespace Mirai {

// TODO(shikugawa): It should be singleton class
class Window : NonCopyable {
 public:
  ~Window() {
    if (initialized_) {
      SDL_DestroyWindow(window_entity_);
    }
  }
  void startWindow() {
    if (initialized_) {
      return;
    }

    window_entity_ = SDL_CreateWindow(name_.c_str(), SDL_WINDOWPOS_UNDEFINED,
                                      SDL_WINDOWPOS_UNDEFINED, width_, height_,
                                      SDL_WINDOW_SHOWN);
    if (!window_entity_) {
      throw MiraiException(
          absl::StrFormat("Failed to create %s window", name_));
    }

    initialized_ = true;
  }
  void setWindowName(std::string&& name) {
    if (!initialized_) {
      name_ = name;
    }
  }
  void setWindowSize(int width, int height) {
    if (!initialized_) {
      width_ = width;
      height_ = height;
    }
  }
  bool initialized() { return initialized_; }

  std::string& name() { return name_; }
  SDL_Window const* windowEntity() { return window_entity_; }

 private:
  SDL_Window* window_entity_;

  bool initialized_{false};
  std::string name_;
  int width_ = 640;
  int height_ = 480;
};

using WindowPtr = std::shared_ptr<Window>;

}  // namespace Mirai

#endif  // Mirai_WINDOW_H
