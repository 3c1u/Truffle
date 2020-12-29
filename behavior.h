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

namespace {
static constexpr bool is_false = false;
}

class TruffleBehavior : public FixedRenderable {
 public:
  TruffleBehavior(Renderer& r, std::string name)
      : FixedRenderable(r, name), name_(name) {}

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

template <class State = NullState>
class ImageTextureBehavior : public TruffleBehavior {
 public:
  explicit ImageTextureBehavior(Renderer& renderer, std::string name, int x,
                                int y)
      : TruffleBehavior(renderer, name), x(x), y(y) {}

  void setStateTransition(State from, State to, ImageTexture& from_texture,
                          ImageTexture& to_texture) {
    state_object_manager_.bindStatefulObject(from, from_texture);
    state_object_manager_.bindStatefulObject(to, to_texture);
    state_object_manager_.defineStateTransition(from, to);
  }

  void setInitState(State init, ImageTexture& texture) {
    state_object_manager_.setInitStatefulObject(
        init, std::shared_ptr<ImageTexture>(&texture));
  }

  // FixedRenderable
  void render() override {
    auto& active_obj = state_object_manager_.activeStateObject();
    SDL_Rect render_rect = {x, y, active_obj.width(), active_obj.height()};
    SDL_RenderCopy(renderer_.entity(), active_obj.entity(),
                   nullptr /* TODO: introduce clip settings */, &render_rect);
  }

 protected:
  StatefulObjectManager<ImageTexture, State> state_object_manager_;
  int x, y;
};

template <>
class ImageTextureBehavior<NullState> : public TruffleBehavior {
 public:
  explicit ImageTextureBehavior(Renderer& renderer, std::string name, int x,
                                int y)
      : TruffleBehavior(renderer, name), x(x), y(y) {}

  void setInitState(ImageTexture& texture) {
    state_object_manager_.setInitStatefulObject(
        std::shared_ptr<ImageTexture>(&texture));
  }

  // FixedRenderable
  void render() override {
    auto& active_obj = state_object_manager_.activeStateObject();
    SDL_Rect render_rect = {x, y, active_obj.width(), active_obj.height()};
    SDL_RenderCopy(renderer_.entity(), active_obj.entity(),
                   nullptr /* TODO: introduce clip settings */, &render_rect);
  }

 protected:
  StatefulObjectManager<ImageTexture, NullState> state_object_manager_;
  int x, y;
};

class TextTextureBehavior : public TruffleBehavior {};

}  // namespace Truffle

#endif  // Truffle_BEHAVIOR_H
