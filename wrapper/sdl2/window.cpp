/**
 * @file      window.cpp
 * @author    Rei Shimizu (shikugawa) <shikugawa@gmail.com>
 * @brief     Low level SDL2 wrapper of window
 *
 * @copyright Copyright 2021 Rei Shimizu. All rights reserved.
 */

#include "window.h"

#include <absl/strings/str_format.h>

#include "common/exception.h"

namespace Truffle {

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