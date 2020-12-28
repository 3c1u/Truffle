//
// Created by shikugawa on 2020/12/19.
//

#ifndef TRUFFLE_TEXTURE_H
#define TRUFFLE_TEXTURE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <absl/strings/str_format.h>

#include <cassert>
#include <string>

#include "exception.h"
#include "renderable.h"
#include "renderer.h"

namespace Truffle {

class ImageTexture {
 public:
  ImageTexture(Renderer& renderer, std::string path, std::string name)
      : name_(name) {
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface) {
      throw TruffleException(
          absl::StrFormat("Failed to load image: %s", path.c_str()));
    }
    texture_ = SDL_CreateTextureFromSurface(renderer.entity(), surface);
    if (!texture_) {
      throw TruffleException(absl::StrFormat(
          "Failed to create texture entity from %s", path.c_str()));
    }
    width_ = surface->w;
    height_ = surface->h;

    SDL_FreeSurface(surface);
  }

  ~ImageTexture() { SDL_DestroyTexture(texture_); }

  SDL_Texture* entity() { return texture_; }

  int width() { return width_; }

  int height() { return height_; }

  const std::string& textureName() { return name_; }

 protected:
  SDL_Texture* texture_;
  int width_;
  int height_;
  std::string name_;
};

}  // namespace Truffle

#endif  // Truffle_TEXTURE_H
