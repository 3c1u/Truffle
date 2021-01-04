/**
 * @file      texture.cpp
 * @author    Rei Shimizu (shikugawa) <shikugawa@gmail.com>
 * @brief
 *
 * @copyright Copyright 2021 Rei Shimizu. All rights reserved.
 */

#include "texture.h"

#include <SDL_image.h>
#include <SDL_ttf.h>
#include <absl/strings/str_format.h>

#include "common/exception.h"
#include "font_storage.h"
#include "renderer_storage.h"

namespace Truffle {

Texture::Texture(std::string path) {
  SDL_Surface* surface = IMG_Load(path.c_str());
  if (!surface) {
    throw TruffleException(
        absl::StrFormat("Failed to load image: %s", path.c_str()));
  }

  texture_ = SDL_CreateTextureFromSurface(
      const_cast<SDL_Renderer*>(
          RendererStorage::get().activeRenderer()->entity()),
      surface);

  if (!texture_) {
    throw TruffleException(absl::StrFormat(
        "Failed to create texture entity from %s", path.c_str()));
  }

  SDL_FreeSurface(surface);
}

Texture::Texture(std::string text, TextTextureMode mode, FontInfo info,
                 Color& fg) {
  SDL_Surface* surface;

  if (mode == TextTextureMode::Blend)
    surface = TTF_RenderText_Blended(
        const_cast<TTF_Font*>(
            FontStorage::get().openFont(info.name, info.size)->entity()),
        text.c_str(), fg);
  else if (mode == TextTextureMode::Solid)
    surface = TTF_RenderText_Solid(
        const_cast<TTF_Font*>(
            FontStorage::get().openFont(info.name, info.size)->entity()),
        text.c_str(), fg);
  else
    assert(false);  // Not supported shaded mode

  if (!surface) {
    throw TruffleException(absl::StrFormat("Failed to text: %s", text.c_str()));
  }

  texture_ = SDL_CreateTextureFromSurface(
      const_cast<SDL_Renderer*>(
          RendererStorage::get().activeRenderer()->entity()),
      surface);
  if (!texture_) {
    throw TruffleException(absl::StrFormat(
        "Failed to create texture entity from %s", text.c_str()));
  }

  SDL_FreeSurface(surface);
}

Texture::~Texture() { SDL_DestroyTexture(texture_); }

}  // namespace Truffle
