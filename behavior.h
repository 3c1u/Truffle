//
// Created by shikugawa on 2020/12/26.
//

#ifndef TRUFFLE_BEHAVIOR_H
#define TRUFFLE_BEHAVIOR_H

#include "renderable.h"
#include "texture.h"

namespace Truffle {

class TruffleBehavior : public FixedRenderable {
 public:
  TruffleBehavior(Renderer& r, std::string name)
      : FixedRenderable(r, name), name_(name) {}

  virtual ~TruffleBehavior() = default;

  virtual void start(){};

  virtual void update(SDL_Event&){};

  const std::string& behaviorName() { return name_; }

 private:
  std::string name_;
};

/**
 * テクスチャに基づくビヘイビアを表現するクラス。このクラスを継承することで動的なテクスチャを表現できる。TruffleBehavior::update()
 * 内で位置の再計算を行い、MoveableRenderable::render(int, int)
 * を呼ぶことでテクスチャの位置変更が出来る。
 *
 * example:
 *
 * class SampleBehavior : public ImageTextureBehavior {
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
 * };
 */
class ImageTextureBehavior : public ImageTexture, public TruffleBehavior {
 public:
  explicit ImageTextureBehavior(Renderer& renderer, std::string path,
                                std::string name, int x, int y)
      : ImageTexture(renderer, path, name),
        TruffleBehavior(renderer, name),
        x(x),
        y(y) {}

  ~ImageTextureBehavior() { SDL_DestroyTexture(texture_); }

  // FixedRenderable
  void render() override {
    SDL_Rect render_rect = {x, y, width_, height_};
    SDL_RenderCopy(renderer_.entity(), texture_,
                   nullptr /* TODO: introduce clip settings */, &render_rect);
  }

 protected:
  int x, y;
};

}  // namespace Truffle

#endif  // Truffle_BEHAVIOR_H
