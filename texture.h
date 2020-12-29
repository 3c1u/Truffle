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
  ImageTexture(Renderer& renderer, std::string path, std::string name);
  ~ImageTexture() { SDL_DestroyTexture(texture_); }

  int width() { return width_; }
  int height() { return height_; }
  SDL_Texture* entity() { return texture_; }
  const std::string& textureName() { return name_; }

 protected:
  SDL_Texture* texture_;
  int width_;
  int height_;
  std::string name_;
};

using ImageTexturePtr = std::shared_ptr<ImageTexture>;

ImageTexture::ImageTexture(Renderer& renderer, std::string path,
                           std::string name)
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

class ImageTextureFactory {
 public:
  ImageTextureFactory(Renderer& r);

  ImageTexturePtr create(std::string path, std::string name);

 private:
  Renderer& renderer_;
};

ImageTextureFactory::ImageTextureFactory(Renderer& r) : renderer_(r) {}

ImageTexturePtr ImageTextureFactory::create(std::string path,
                                            std::string name) {
  return std::make_shared<ImageTexture>(renderer_, path, name);
}

// https://www.libsdl.org/projects/SDL_ttf/docs/SDL_ttf_42.html#SEC42
enum class TextTextureMode { Solid, Blend, Shaded };

class TextTexture {
 public:
  TextTexture(Renderer& renderer, Font& font, std::string name);
  ~TextTexture() { SDL_DestroyTexture(texture_); }

  void loadSolidTexture(std::string text, Color& fg);
  void loadBlendTexture(std::string text, Color& fg);
  void loadShadedTexture(std::string text, Color& fg, Color& bg);

  int width() { return width_; }
  int height() { return height_; }
  SDL_Texture* entity() { return texture_; }
  const std::string& textureName() { return name_; }

 private:
  Renderer& renderer_;
  Font& font_;
  SDL_Texture* texture_;
  int width_;
  int height_;
  std::string name_;
};

TextTexture::TextTexture(Renderer& renderer, Font& font, std::string name)
    : renderer_(renderer), font_(font), name_(name) {}

void TextTexture::loadSolidTexture(std::string text, Color& fg) {
  if (!texture_) {
    SDL_DestroyTexture(texture_);
  }
  SDL_Surface* surface = TTF_RenderText_Solid(font_.entity(), text.c_str(), fg);
  if (!surface) {
    throw TruffleException(absl::StrFormat("Failed to text: %s", text.c_str()));
  }
  texture_ = SDL_CreateTextureFromSurface(renderer_.entity(), surface);
  if (!texture_) {
    throw TruffleException(absl::StrFormat(
        "Failed to create texture entity from %s", text.c_str()));
  }
  width_ = surface->w;
  height_ = surface->h;
  SDL_FreeSurface(surface);
}

void TextTexture::loadBlendTexture(std::string text, Color& fg) {
  if (!texture_) {
    SDL_DestroyTexture(texture_);
  }
  SDL_Surface* surface =
      TTF_RenderText_Blended(font_.entity(), text.c_str(), fg);
  if (!surface) {
    throw TruffleException(absl::StrFormat("Failed to text: %s", text.c_str()));
  }
  texture_ = SDL_CreateTextureFromSurface(renderer_.entity(), surface);
  if (!texture_) {
    throw TruffleException(absl::StrFormat(
        "Failed to create texture entity from %s", text.c_str()));
  }
  width_ = surface->w;
  height_ = surface->h;
  SDL_FreeSurface(surface);
}

void TextTexture::loadShadedTexture(std::string text, Color& fg, Color& bg) {
  if (!texture_) {
    SDL_DestroyTexture(texture_);
  }
  SDL_Surface* surface =
      TTF_RenderText_Shaded(font_.entity(), text.c_str(), fg, bg);
  if (!surface) {
    throw TruffleException(absl::StrFormat("Failed to text: %s", text.c_str()));
  }
  texture_ = SDL_CreateTextureFromSurface(renderer_.entity(), surface);
  if (!texture_) {
    throw TruffleException(absl::StrFormat(
        "Failed to create texture entity from %s", text.c_str()));
  }
  width_ = surface->w;
  height_ = surface->h;
  SDL_FreeSurface(surface);
}

using TextTexturePtr = std::shared_ptr<TextTexture>;

class TextTextureFactory {
 public:
  TextTextureFactory(Renderer& r, Font& font);

  TextTexturePtr create(std::string name);

 private:
  Font& font_;
  Renderer& renderer_;
};

TextTextureFactory::TextTextureFactory(Renderer& r, Font& font)
    : font_(font), renderer_(renderer_) {}

TextTexturePtr TextTextureFactory::create(std::string name) {
  return std::make_shared<TextTexture>(renderer_, font_, name);
}

}  // namespace Truffle

#endif  // Truffle_TEXTURE_H
