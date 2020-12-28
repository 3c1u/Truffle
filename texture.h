//
// Created by shikugawa on 2020/12/19.
//

#ifndef TRUFFLE_TEXTURE_H
#define TRUFFLE_TEXTURE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <absl/strings/str_format.h>

#include <cassert>
#include <string>

#include "color.h"
#include "exception.h"
#include "font.h"
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

class TextTexture {
 public:
  // https://www.libsdl.org/projects/SDL_ttf/docs/SDL_ttf_42.html#SEC42
  enum class Mode {
    Solid,
    Shaded,
    Blended,
  };

  TextTexture(Renderer& renderer, Font& font, std::string text, Color& color_fg,
              Mode mode, std::string name)
      : name_(name) {
    SDL_Surface* surface;
    if (mode == Mode::Solid) {
      surface = TTF_RenderText_Solid(font.entity(), text.c_str(), color_fg);
    } else if (mode == Mode::Blended) {
      surface = TTF_RenderText_Blended(font.entity(), text.c_str(), color_fg);
    } else {
      throw TruffleException("Failed to load surface with this mode");
    }
    if (!surface) {
      throw TruffleException(
          absl::StrFormat("Failed to text: %s", text.c_str()));
    }
    texture_ = SDL_CreateTextureFromSurface(renderer.entity(), surface);
    if (!texture_) {
      throw TruffleException(absl::StrFormat(
          "Failed to create texture entity from %s", text.c_str()));
    }
    width_ = surface->w;
    height_ = surface->h;

    SDL_FreeSurface(surface);
  }

  TextTexture(Renderer& renderer, Font& font, std::string text, Color& color_fg,
              Color& color_bg, Mode mode, std::string name) {
    SDL_Surface* surface;
    if (mode == Mode::Shaded) {
      surface = TTF_RenderText_Shaded(font.entity(), text.c_str(), color_fg,
                                      color_bg);
    } else {
      throw TruffleException("Failed to load surface with this mode");
    }
    if (!surface) {
      throw TruffleException(
          absl::StrFormat("Failed to text: %s", text.c_str()));
    }
    texture_ = SDL_CreateTextureFromSurface(renderer.entity(), surface);
    if (!texture_) {
      throw TruffleException(absl::StrFormat(
          "Failed to create texture entity from %s", text.c_str()));
    }
    width_ = surface->w;
    height_ = surface->h;

    SDL_FreeSurface(surface);
  }

  ~TextTexture() { SDL_DestroyTexture(texture_); }

  SDL_Texture* entity() { return texture_; }

  int width() { return width_; }

  int height() { return height_; }

  const std::string& textureName() { return name_; }

 private:
  SDL_Texture* texture_;
  int width_;
  int height_;
  std::string name_;
};

}  // namespace Truffle

#endif  // Truffle_TEXTURE_H
