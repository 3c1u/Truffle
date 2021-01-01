//
// Created by shikugawa on 2020/12/26.
//

#ifndef TRUFFLE_SCENE_H
#define TRUFFLE_SCENE_H

#include <absl/container/flat_hash_map.h>

#include <forward_list>
#include <memory>
#include <queue>

#include "bus.h"
#include "message.h"
#include "renderable.h"
#include "stateful_object_manager.h"
#include "texture.h"

namespace Truffle {

class Scene;

class ButtonBase : public Renderable {
 public:
  ButtonBase(Scene& parent_scene, const Renderer& renderer, std::string name);

  // Renderable
  virtual void render() override {}

  /**
   * ボタンが押された時のコールバック
   */
  virtual void onButtonPressed() = 0;

  /**
   * ボタンが離された時のコールバック
   */
  virtual void onButtonReleased() = 0;

  /**
   * ボタンがホバーされた時のコールバック
   */
  virtual void onMouseHovered() = 0;

  /**
   * ボタンがアンホバーされた時のコールバック
   */
  virtual void onMouseUnhovered() = 0;

  /**
   * 同一シーン内のビヘイビアにメッセージを送る。メッセージの送信に失敗した場合は例外を送出する。
   * 失敗した場合はfalseを返す。
   * @param dst_behavior
   * @param msg
   * @return
   */
  bool sendMessage(std::string dst_behavior, const Message& msg);
  bool sendMessage(std::string dst_behavior, Message&& msg);

  virtual void _onButtonPressed(SDL_Event& ev) = 0;
  virtual void _onButtonReleased(SDL_Event& ev) = 0;
  virtual void _onMouseHovered() = 0;
  virtual void _onMouseUnhovered() = 0;

  [[nodiscard]] const std::string& name() const& { return name_; }

 private:
  const std::string name_;
  Scene& parent_scene_;
};

class TruffleBehavior {
 public:
  /**
   * ビヘイビアのコンストラクタ
   * @param parent_scene ビヘイビアが所属するシーンの参照
   * @param name ビヘイビアの名前。名前に重複があると例外が発生するので注意。
   */
  TruffleBehavior(Scene& parent_scene, std::string name);

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
   * Sceneで発行されたメッセージキューのポインタを保持する。
   * @param message_queue_ptr
   */
  void setMessageQueue(std::shared_ptr<std::queue<Message>> message_queue_ptr) {
    message_queue_ = std::move(message_queue_ptr);
  }

  /**
   * メッセージを1件受け取る
   * @return
   */
  std::optional<Message> recvMessage();

  [[nodiscard]] const std::string& name() const& { return name_; }
  [[nodiscard]] const std::forward_list<std::reference_wrapper<Renderable>>&
  targetRenderables() const& {
    return renderables_;
  }

 private:
  Scene& parent_scene_;
  std::forward_list<std::reference_wrapper<Renderable>> renderables_;
  std::shared_ptr<std::queue<Message>> message_queue_;
  std::string name_;
};

class Scene : NonCopyable {
 public:
  explicit Scene(std::string scene_name);

  /**
   * シーンの初期化を行う。
   */
  void initScene() const&;

  /**
   * 同一シーン内のビヘイビアにメッセージを送る。メッセージの送信に失敗した場合は例外を送出する。
   * @tparam T
   * @param dst_behavior
   * @param message
   * @return
   */
  template <class T>
  bool sendMessage(std::string dst_behavior, T&& message) {
    return bus_.sendMessage(dst_behavior, std::forward<T&&>(message));
  }

  [[nodiscard]] const std::string& name() const& { return name_; }
  [[nodiscard]] const absl::flat_hash_map<
      std::string, std::reference_wrapper<TruffleBehavior>>&
  behaviors() const& {
    return behaviors_;
  }
  [[nodiscard]] const absl::flat_hash_map<std::string,
                                          std::reference_wrapper<ButtonBase>>&
  buttons() const& {
    return buttons_;
  }

 private:
  friend class TruffleBehavior;
  friend class ButtonBase;

  void setButton(ButtonBase& b);
  void setBehavior(TruffleBehavior& b);

  std::string name_;

  EventMessageBus& bus_;
  absl::flat_hash_map<std::string, std::reference_wrapper<TruffleBehavior>>
      behaviors_;
  absl::flat_hash_map<std::string, std::reference_wrapper<ButtonBase>> buttons_;
};

TruffleBehavior::TruffleBehavior(Scene& parent_scene, std::string name)
    : parent_scene_(parent_scene), name_(name) {
  parent_scene_.setBehavior(*this);
}

std::optional<Message> TruffleBehavior::recvMessage() {
  if (message_queue_->empty()) {
    return std::nullopt;
  }
  auto message = message_queue_->front();
  message_queue_->pop();
  return message;
}

ButtonBase::ButtonBase(Scene& parent_scene, const Renderer& renderer,
                       std::string name)
    : Renderable(renderer), name_(std::move(name)), parent_scene_(parent_scene) {
  parent_scene_.setButton(*this);
}

bool ButtonBase::sendMessage(std::string dst_behavior, const Message& msg) {
  parent_scene_.sendMessage(std::move(dst_behavior), std::forward<const Message&>(msg));
}

bool ButtonBase::sendMessage(std::string dst_behavior, Message&& msg) {
  parent_scene_.sendMessage(std::move(dst_behavior), std::forward<Message&&>(msg));
}

Scene::Scene(std::string scene_name)
    : name_(std::move(scene_name)), bus_(EventMessageBus::get()) {}

void Scene::initScene() const& {
  for (const auto& [_, cb] : behaviors_) {
    cb.get().start();
  }
}

void Scene::setBehavior(TruffleBehavior& b) {
  if (behaviors_.find(b.name()) != behaviors_.end()) {
    throw TruffleException(
        absl::StrFormat("behavior %s had already registered", b.name()));
  }
  log(LogLevel::INFO,
      absl::StrFormat("behavior %s registered to scene %s", b.name(), name_));
  auto queue = bus_.getMessageQueue(b.name());
  b.setMessageQueue(queue);
  behaviors_.emplace(b.name(), b);
}

void Scene::setButton(ButtonBase& b) {
  if (buttons_.find(b.name()) != buttons_.end()) {
    throw TruffleException(
        absl::StrFormat("button %s had already registered", b.name()));
  }
  log(LogLevel::INFO,
      absl::StrFormat("button %s registered to scene %s", b.name(), name_));
  buttons_.emplace(b.name(), b);
}

}  // namespace Truffle

#endif  // Truffle_BEHAVIOR_H
