//
// Created by shikugawa on 2020/12/26.
//

#ifndef TRUFFLE_BEHAVIOR_H
#define TRUFFLE_BEHAVIOR_H

#include <memory>

#include "renderable.h"
#include "stateful_object_manager.h"
#include "texture.h"

namespace Truffle {

class TruffleBehavior : public Renderable {
 public:
  TruffleBehavior(Renderer& r, std::string name)
      : Renderable(r, name), name_(name) {}

  virtual ~TruffleBehavior() = default;

  /**
   * シーンの開始時に一度だけ実行されるコールバック
   */
  virtual void start(){};

  /**
   * 毎フレーム毎に1回呼ばれるコールバック
   */
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
 *     SampleBehavior(...) : ImageTextureBehavior(...) {}
 *
 *     void start() override {
 *         x_ = 0;
 *         y_ = 0;
 *     }
 *
 *     void update() override {
 *         x_ += 40;
 *         y_ += 40;
 *     }
 * };
 */

template <class State>
class ImageTextureBehavior : public TruffleBehavior {
 public:
  explicit ImageTextureBehavior(Renderer& renderer, std::string name, int x,
                                int y)
      : TruffleBehavior(renderer, name),
        image_texture_factory(renderer),
        x(x),
        y(y) {}

  void setInitTexture(State init, ImageTexturePtr texture) {
    if (init_) {
      throw TruffleException(absl::StrFormat(
          "%s: image texture can't be initialized twice", behaviorName()));
    }
    state_object_manager.setInitStatefulObject(init, texture);
    init_ = true;
  }

  // FixedRenderable
  void render() override final {
    if (!init_) {
      throw TruffleException(absl::StrFormat(
          "%s: image texture must be initialized", behaviorName()));
    }
    auto& active_obj = state_object_manager.activeStateObject();
    SDL_Rect render_rect = {x, y, active_obj.width(), active_obj.height()};
    SDL_RenderCopy(renderer_.entity(), active_obj.entity(),
                   nullptr /* TODO: introduce clip settings */, &render_rect);
  }

 protected:
  bool isMouseHovered() {
    int mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);
    if ((x < mouse_x &&
         mouse_x < x + state_object_manager.activeStateObject().width()) &&
        (y < mouse_y &&
         mouse_y < y + state_object_manager.activeStateObject().height())) {
      return true;
    }
    return false;
  }

  StatefulObjectManager<ImageTexture, State> state_object_manager;
  ImageTextureFactory image_texture_factory;
  int x, y;

 private:
  bool init_ = false;
};

template <>
class ImageTextureBehavior<NullState> : public TruffleBehavior {
 public:
  explicit ImageTextureBehavior(Renderer& renderer, std::string name, int x,
                                int y)
      : TruffleBehavior(renderer, name),
        image_texture_factory_(renderer),
        x(x),
        y(y) {}

  void setInitTexture(std::string path, std::string name) {
    state_object_manager_.setInitStatefulObject(
        image_texture_factory_.create(path, name));
  }

  // FixedRenderable
  void render() override final {
    auto& texture_ = state_object_manager_.activeStateObject();
    SDL_Rect render_rect = {x, y, texture_.width(), texture_.height()};
    SDL_RenderCopy(renderer_.entity(), texture_.entity(),
                   nullptr /* TODO: introduce clip settings */, &render_rect);
  }

 protected:
  int x, y;

 private:
  ImageTextureFactory image_texture_factory_;
  StatefulObjectManager<ImageTexture, NullState> state_object_manager_;
};

class TextTextureBehavior : public TruffleBehavior {};

}  // namespace Truffle

#endif  // Truffle_BEHAVIOR_H
