//
// Created by shikugawa on 2020/12/28.
//

#ifndef TRUFFLE_FONT_H
#define TRUFFLE_FONT_H

#include <SDL2/SDL_ttf.h>
#include <absl/strings/str_format.h>

#include "exception.h"
#include "non_copyable.h"

namespace Truffle {

class Font : NonCopyable {
 public:
  Font(std::string path, size_t size) {
    font_ = TTF_OpenFont(path.c_str(), size);
    if (!font_) {
      throw TruffleException(absl::StrFormat("Failed to load font %s", path));
    }
  }

  ~Font() { TTF_CloseFont(font_); }

  [[nodiscard]] TTF_Font const* entity() const& { return font_; }

 private:
  TTF_Font* font_;
};

}  // namespace Truffle

#endif  // TRUFFLE_FONT_H
