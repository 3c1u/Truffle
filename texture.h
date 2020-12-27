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

/**
 * 位置が変動するテクスチャを表現するクラス。このクラスを継承することで動的なテクスチャを表現できる。TruffleBehavior::update()
 * 内で位置の再計算を行い、MoveableRenderable::render(int, int)
 * を呼ぶことでテクスチャの位置変更が出来る。
 *
 * example:
 *
 * class SampleBehavior : public DynamicImageTextureBehavior {
 * public:
 *     int x_;
 *     int y_;
 *
 *     void start() {
 *         x_ = 0;
 *         y_ = 0;
 *     }
 *     void update() {
 *         x_ += 40;
 *         y_ += 40;
 *         render(x, y);
 *     }
 *     void onMouseButtonClicked(SDL_Event& ev) {}
 * };
 */
class DynamicImageTextureBehavior : public MovableRenderable,
                                    public TruffleBehavior {
 public:
  explicit DynamicImageTextureBehavior(Renderer& renderer, std::string path,
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

  ~DynamicImageTextureBehavior() { SDL_DestroyTexture(texture_); }

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

/**
 * 位置が変動しないテクスチャを表現するクラス。このクラスを継承することで静的なテクスチャを表現できる。
 *
 * example:
 *
 * class SampleBehavior : public StaticImageTextureBehavior {
 * public:
 *     void start() {}
 *     void update() {
 *         render();
 *     }
 *     void onMouseButtonClicked(SDL_Event& ev) {}
 */
class StaticImageTextureBehavior : public FixedRenderable,
                                   public TruffleBehavior {
 public:
  explicit StaticImageTextureBehavior(Renderer& renderer, std::string path,
                                      std::string name, int x, int y)
      : FixedRenderable(renderer, name), TruffleBehavior(name), x_(x), y_(y) {
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

  ~StaticImageTextureBehavior() { SDL_DestroyTexture(texture_); }

  // FixedRenderable
  void render() override {
    SDL_Rect render_rect = {x_, y_, width_, height_};
    SDL_RenderCopy(renderer_.entity(), texture_,
                   nullptr /* TODO: introduce clip settings */, &render_rect);
  }

 private:
  SDL_Texture* texture_;
  const int x_;
  const int y_;
  int width_;
  int height_;
};

}  // namespace Truffle

#endif  // Truffle_TEXTURE_H
