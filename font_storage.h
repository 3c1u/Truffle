/**
 * @file      font_storage.h
 * @author    Hikaru Terazono (3c1u) <3c1u@vulpesgames.tokyo>
 * @brief     Font storage type. Wrapper of sdl_ttf
 *
 * @copyright Copyright 2021 Hikaru Terazono. All rights reserved.
 */

#pragma once

#include <SDL2/SDL_ttf.h>

#include <memory>
#include <string>

#include "exception.h"
#include "font.h"
#include "logger.h"
#include "non_copyable.h"
#include "singleton.h"

namespace Truffle {

class FontStorage final : public ConstSingleton<FontStorage>,
                          public NonCopyable {
 public:
  static std::unique_ptr<Font> openFont(std::string const& path, size_t size) {
    return FontStorage::get().openFont_(path, size);
  }

 private:
  friend class ConstSingleton<FontStorage>;

  FontStorage() {
    // SDL2_ttf should be initialized once
    log(LogLevel::DEBUG, "SDL_ttf initialized");
    TTF_Init();
  }

  std::unique_ptr<Font> openFont_(std::string const& path, size_t size) const {
    return std::make_unique<Font>(path, size);
  }
};

}  // namespace Truffle
