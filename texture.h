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
#include "object_tree.h"
#include "renderer.h"

namespace Truffle {

class ImageTexture : public TruffleObject {
 public:
  ImageTexture(TruffleController& parent_controller, const Renderer& renderer,
               std::string path, std::string name, int x, int y);
  ~ImageTexture() { SDL_DestroyTexture(texture_); }

  [[nodiscard]] SDL_Texture const* entity() const& { return texture_; }

  // Renderable
  void render() final;

 private:
  SDL_Texture* texture_;
};

ImageTexture::ImageTexture(TruffleController& parent_controller,
                           const Renderer& renderer, std::string path,
                           std::string name, int x, int y)
    : TruffleObject(parent_controller, renderer, name) {
  SDL_Surface* surface = IMG_Load(path.c_str());
  if (!surface) {
    throw TruffleException(
        absl::StrFormat("Failed to load image: %s", path.c_str()));
  }
  texture_ = SDL_CreateTextureFromSurface(
      const_cast<SDL_Renderer*>(renderer.entity()), surface);
  if (!texture_) {
    throw TruffleException(absl::StrFormat(
        "Failed to create texture entity from %s", path.c_str()));
  }

  setPoint(x, y);
  setWidth(surface->w);
  setHeight(surface->h);

  SDL_FreeSurface(surface);
}

void ImageTexture::render() {
  if (do_render_) {
    SDL_RenderCopy(const_cast<SDL_Renderer*>(renderer_.entity()), texture_,
                   nullptr /* TODO: introduce clip settings */, &renderRect());
  }
}

// https://www.libsdl.org/projects/SDL_ttf/docs/SDL_ttf_42.html#SEC42
enum class TextTextureMode { Solid, Blend, Shaded };

class TextTexture : public TruffleObject {
 public:
  TextTexture(TruffleController& parent_controller, const Renderer& renderer,
              const Font& font, std::string name, int x, int y);
  ~TextTexture() { SDL_DestroyTexture(texture_); }

  void loadSolidTexture(std::string text, Color& fg);
  void loadBlendTexture(std::string text, Color& fg);
  void loadShadedTexture(std::string text, Color& fg, Color& bg);

  [[nodiscard]] SDL_Texture const* entity() const& { return texture_; }

  // Renderable
  void render() final;

 private:
  const Font& font_;
  SDL_Texture* texture_;
};

TextTexture::TextTexture(TruffleController& parent_controller,
                         const Renderer& renderer, const Font& font,
                         std::string name, int x, int y)
    : TruffleObject(parent_controller, renderer, name), font_(font) {
  setPoint(x, y);
}

void TextTexture::loadSolidTexture(std::string text, Color& fg) {
  if (!texture_) {
    SDL_DestroyTexture(texture_);
  }
  SDL_Surface* surface = TTF_RenderText_Solid(
      const_cast<TTF_Font*>(font_.entity()), text.c_str(), fg);
  if (!surface) {
    throw TruffleException(absl::StrFormat("Failed to text: %s", text.c_str()));
  }
  texture_ = SDL_CreateTextureFromSurface(
      const_cast<SDL_Renderer*>(renderer_.entity()), surface);
  if (!texture_) {
    throw TruffleException(absl::StrFormat(
        "Failed to create texture entity from %s", text.c_str()));
  }

  setWidth(surface->w);
  setHeight(surface->h);

  SDL_FreeSurface(surface);
}

void TextTexture::loadBlendTexture(std::string text, Color& fg) {
  if (!texture_) {
    SDL_DestroyTexture(texture_);
  }
  SDL_Surface* surface = TTF_RenderText_Blended(
      const_cast<TTF_Font*>(font_.entity()), text.c_str(), fg);
  if (!surface) {
    throw TruffleException(absl::StrFormat("Failed to text: %s", text.c_str()));
  }
  texture_ = SDL_CreateTextureFromSurface(
      const_cast<SDL_Renderer*>(renderer_.entity()), surface);
  if (!texture_) {
    throw TruffleException(absl::StrFormat(
        "Failed to create texture entity from %s", text.c_str()));
  }
  setWidth(surface->w);
  setHeight(surface->h);

  SDL_FreeSurface(surface);
}

void TextTexture::loadShadedTexture(std::string text, Color& fg, Color& bg) {
  if (!texture_) {
    SDL_DestroyTexture(texture_);
  }
  SDL_Surface* surface = TTF_RenderText_Shaded(
      const_cast<TTF_Font*>(font_.entity()), text.c_str(), fg, bg);
  if (!surface) {
    throw TruffleException(absl::StrFormat("Failed to text: %s", text.c_str()));
  }
  texture_ = SDL_CreateTextureFromSurface(
      const_cast<SDL_Renderer*>(renderer_.entity()), surface);
  if (!texture_) {
    throw TruffleException(absl::StrFormat(
        "Failed to create texture entity from %s", text.c_str()));
  }
  setWidth(surface->w);
  setHeight(surface->h);

  SDL_FreeSurface(surface);
}

void TextTexture::render() {
  if (do_render_) {
    SDL_RenderCopy(const_cast<SDL_Renderer*>(renderer_.entity()), texture_,
                   nullptr /* TODO: introduce clip settings */, &renderRect());
  }
}

}  // namespace Truffle

#endif  // Truffle_TEXTURE_H
