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

#include "behavior.h"
#include "exception.h"
#include "renderable.h"
#include "renderer.h"

namespace Truffle {

class ImageTextureBehavior : public MovableRenderable, public TruffleBehavior {
 public:
  explicit ImageTextureBehavior(Renderer& renderer, std::string path,
                                std::string name)
      : MovableRenderable(renderer, name), TruffleBehavior(name) {
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface) {
      throw TruffleException(
          absl::StrFormat("Failed to load image: %s", path.c_str()));
    }
    texture_ = SDL_CreateTextureFromSurface(renderer_.entity(), surface);
    if (!texture_) {
      throw TruffleException(absl::StrFormat(
          "Failed to create texture entity from %s", path.c_str()));
    }
    width_ = surface->w;
    height_ = surface->h;

    SDL_FreeSurface(surface);
  }

  ~ImageTextureBehavior() { SDL_DestroyTexture(texture_); }

  // MovableRenderable
  void render(int x, int y) override {
    SDL_Rect render_rect = {x, y, width_, height_};
    SDL_RenderCopy(renderer_.entity(), texture_,
                   nullptr /* TODO: introduce clip settings */, &render_rect);
  }

 private:
  SDL_Texture* texture_;
  int width_;
  int height_;
};

}  // namespace Truffle

#endif  // Truffle_TEXTURE_H
