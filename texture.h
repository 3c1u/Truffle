//
// Created by shikugawa on 2020/12/19.
//

#ifndef MIRAI_TEXTURE_H
#define MIRAI_TEXTURE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <absl/strings/str_format.h>

#include <cassert>
#include <string>

#include "behavior.h"
#include "exception.h"
#include "renderable.h"
#include "renderer.h"

namespace Mirai {

class ImageTextureBehavior : public Renderable, public MiraiBehavior {
 public:
  explicit ImageTextureBehavior(std::string path, Renderer& renderer)
      : parent_renderer_(renderer) {
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface) {
      throw MiraiException(
          absl::StrFormat("Failed to load image: %s", path.c_str()));
    }
    texture_ = SDL_CreateTextureFromSurface(parent_renderer_.entity(), surface);
    if (!texture_) {
      throw MiraiException(absl::StrFormat(
          "Failed to create texture entity from %s", path.c_str()));
    }
    width_ = surface->w;
    height_ = surface->h;

    SDL_FreeSurface(surface);
  }

  ~ImageTextureBehavior() { SDL_DestroyTexture(texture_); }

  // Renderable
  void render(int x, int y) override {
    SDL_Rect render_rect = {x, y, width_, height_};
    SDL_RenderCopy(parent_renderer_.entity(), texture_,
                   nullptr /* TODO: introduce clip settings */, &render_rect);
  }

 private:
  Renderer& parent_renderer_;
  SDL_Texture* texture_;
  int width_;
  int height_;
};

}  // namespace Mirai

#endif  // Mirai_TEXTURE_H
