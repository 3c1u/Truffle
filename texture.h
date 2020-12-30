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

class ImageTexture : public Renderable {
 public:
  ImageTexture(Renderer& renderer, std::string path, std::string name, int x,
               int y);
  ~ImageTexture() { SDL_DestroyTexture(texture_); }

  int width() { return width_; }
  int height() { return height_; }
  SDL_Texture* entity() { return texture_; }
  const std::string& textureName() { return name_; }

  // Renderable
  void render() override final;

  int x, y;

 private:
  SDL_Texture* texture_;
  int width_;
  int height_;
  int x_;
  int y_;
  std::string name_;
};

using ImageTexturePtr = std::shared_ptr<ImageTexture>;

ImageTexture::ImageTexture(Renderer& renderer, std::string path,
                           std::string name, int x, int y)
    : Renderable(renderer), name_(name), x(x), y(y) {
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

void ImageTexture::render() {
  if (do_render_) {
    SDL_Rect render_rect = {x, y, width_, height_};
    SDL_RenderCopy(renderer_.entity(), texture_,
                   nullptr /* TODO: introduce clip settings */, &render_rect);
  }
}

class ImageTextureFactory {
 public:
  ImageTextureFactory(Renderer& r);

  ImageTexturePtr create(std::string path, std::string name, int x, int y);

 private:
  Renderer& renderer_;
};

ImageTextureFactory::ImageTextureFactory(Renderer& r) : renderer_(r) {}

ImageTexturePtr ImageTextureFactory::create(std::string path, std::string name,
                                            int x, int y) {
  return std::make_shared<ImageTexture>(renderer_, path, name, x, y);
}

// https://www.libsdl.org/projects/SDL_ttf/docs/SDL_ttf_42.html#SEC42
enum class TextTextureMode { Solid, Blend, Shaded };

class TextTexture : public Renderable {
 public:
  TextTexture(Renderer& renderer, Font& font, std::string name, int x, int y);
  ~TextTexture() { SDL_DestroyTexture(texture_); }

  void loadSolidTexture(std::string text, Color& fg);
  void loadBlendTexture(std::string text, Color& fg);
  void loadShadedTexture(std::string text, Color& fg, Color& bg);

  int width() { return width_; }
  int height() { return height_; }
  SDL_Texture* entity() { return texture_; }
  const std::string& textureName() { return name_; }

  // Renderable
  void render() override final;

  int x, y;

 private:
  Font& font_;
  SDL_Texture* texture_;
  int width_;
  int height_;
  std::string name_;
};

TextTexture::TextTexture(Renderer& renderer, Font& font, std::string name,
                         int x, int y)
    : Renderable(renderer), font_(font), name_(name), x(x), y(y) {}

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

void TextTexture::render() {
  if (do_render_) {
    SDL_Rect render_rect = {x, y, width_, height_};
    SDL_RenderCopy(renderer_.entity(), texture_,
                   nullptr /* TODO: introduce clip settings */, &render_rect);
  }
}

using TextTexturePtr = std::shared_ptr<TextTexture>;

class TextTextureFactory {
 public:
  TextTextureFactory(Renderer& r, Font& font);

  TextTexturePtr create(std::string name, int x, int y);

 private:
  Font& font_;
  Renderer& renderer_;
};

TextTextureFactory::TextTextureFactory(Renderer& r, Font& font)
    : font_(font), renderer_(r) {}

TextTexturePtr TextTextureFactory::create(std::string name, int x, int y) {
  return std::make_shared<TextTexture>(renderer_, font_, name, x, y);
}

}  // namespace Truffle

#endif  // Truffle_TEXTURE_H
