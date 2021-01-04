/**
 * @file      texture.h
 * @author    Rei Shimizu (shikugawa) <shikugawa@gmail.com>
 * @brief     SDL Texture wrapper
 *
 * @copyright Copyright 2021 Rei Shimizu. All rights reserved.
 */

#ifndef TRUFFLE_TEXTURE_H
#define TRUFFLE_TEXTURE_H

#include <SDL.h>

#include <string>

namespace Truffle {

// https://www.libsdl.org/projects/SDL_ttf/docs/SDL_ttf_42.html#SEC42
enum class TextTextureMode { Solid, Blend, Shaded };

struct FontInfo {
  size_t size;
  std::string name;
};

class Texture {
 public:
  Texture(std::string path);
  Texture(std::string text, TextTextureMode mode, FontInfo info, Color& fg);
  ~Texture();

  [[nodiscard]] SDL_Texture const* entity() const& { return texture_; }

 private:
  SDL_Texture* texture_;
};

}  // namespace Truffle

#endif  // TRUFFLE_TEXTURE_H
