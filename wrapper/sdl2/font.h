/**
 * @file      font.h
 * @author    Rei Shimizu (shikugawa) <shikugawa@gmail.com>
 * @brief     Low level SDL2 font wrapper
 *
 * @copyright Copyright 2021 Rei Shimizu. All rights reserved.
 */

#ifndef TRUFFLE_FONT_H
#define TRUFFLE_FONT_H

#include <SDL2/SDL_ttf.h>
#include <absl/strings/str_format.h>

#include <string>

#include "common/exception.h"
#include "common/logger.h"
#include "common/non_copyable.h"
#include "common/singleton.h"

namespace Truffle {

class Font : NonCopyable {
 public:
  Font(std::string path, size_t size) {
    font_ = TTF_OpenFont(path.c_str(), size);
    if (!font_) {
      throw TruffleException(absl::StrFormat("Failed to load font %s", path));
    }
  }

  Font(TTF_Font* font) : font_(font) {
    if (!font) {
      throw TruffleException(
          "attempt to initialize Truffle::Font with an invalid TTF_Font "
          "reference");
    }
  }

  ~Font() {
    if (font_) {
      TTF_CloseFont(font_);
    }
  }

  [[nodiscard]] TTF_Font const* entity() const& { return font_; }

 private:
  TTF_Font* font_;
};

}  // namespace Truffle

#endif  // TRUFFLE_FONT_H
