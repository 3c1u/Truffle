//
// Created by shikugawa on 2020/12/26.
//

#ifndef TRUFFLE_BEHAVIOR_H
#define TRUFFLE_BEHAVIOR_H

#include <forward_list>
#include <memory>

#include "renderable.h"
#include "stateful_object_manager.h"
#include "texture.h"

namespace Truffle {

class TruffleBehavior {
 public:
  TruffleBehavior(std::string name) : name_(name) {}

  virtual ~TruffleBehavior() = default;

  /**
   * シーンの開始時に一度だけ実行されるコールバック
   */
  virtual void start(){};

  /**
   * 毎フレーム毎に1回呼ばれるコールバック
   */
  virtual void update(SDL_Event&){};

  /**
   * 描画可能オブジェクトを追加する
   * @param renderable
   */
  void addRenderable(Renderable& renderable) {
    renderables_.push_front(renderable);
  }

  /**
   * メッセージキューを登録する
   * @param message_queue_ptr
   */
  void setMessageQueue(std::shared_ptr<std::queue<Message>> message_queue_ptr) {
    message_queue_ = message_queue_ptr;
  }

  [[nodiscard]] const std::string& name() const& { return name_; }
  [[nodiscard]] const std::forward_list<std::reference_wrapper<Renderable>>&
  targetRenderables() const& {
    return renderables_;
  }

 private:
  std::forward_list<std::reference_wrapper<Renderable>> renderables_;
  std::shared_ptr<std::queue<Message>> message_queue_;
  std::string name_;
};

}  // namespace Truffle

#endif  // Truffle_BEHAVIOR_H
