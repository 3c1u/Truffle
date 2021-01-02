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
#include "object.h"
#include "renderable.h"
#include "stateful_object_manager.h"
#include "texture.h"

namespace Truffle {

class Scene;

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
   * オブジェクトを追加する
   * @param renderable
   */
  void addObject(Object& object) { objects_.push_front(object); }

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
  [[nodiscard]] const std::forward_list<std::reference_wrapper<Object>>&
  targetObjects() const& {
    return objects_;
  }

 private:
  Scene& parent_scene_;
  std::forward_list<std::reference_wrapper<Object>> objects_;
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

 private:
  friend class TruffleBehavior;

  void setBehavior(TruffleBehavior& b);

  std::string name_;

  EventMessageBus& bus_;
  absl::flat_hash_map<std::string, std::reference_wrapper<TruffleBehavior>>
      behaviors_;
};

TruffleBehavior::TruffleBehavior(Scene& parent_scene, std::string name)
    : parent_scene_(parent_scene), name_(name) {
  parent_scene_.setBehavior(*this);
}

std::optional<Message> TruffleBehavior::recvMessage() {
  assert(message_queue_ != nullptr);
  if (message_queue_->empty()) {
    return std::nullopt;
  }
  auto message = message_queue_->front();
  message_queue_->pop();
  return message;
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

}  // namespace Truffle

#endif  // Truffle_BEHAVIOR_H
