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
#include "renderable.h"

namespace Truffle {

class TruffleBehavior;
class Object;

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
   * オブジェクトを追加する。名前が重複していれば例外を発生する。
   * @param renderable
   */
  void addObject(Object& object);

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

  /**
   * メッセージを1件送る
   * @tparam T
   * @param dst_behavior
   * @param message
   */
  template <class T>
  void sendMessage(std::string dst_behavior, T&& message) {
    parent_scene_.sendMessage(dst_behavior, std::forward<T&&>(message));
  }

  [[nodiscard]] const std::string& name() const& { return name_; }
  [[nodiscard]] const absl::flat_hash_map<std::string,
                                          std::reference_wrapper<Object>>&
  targetObjects() const& {
    return objects_;
  }

 private:
  Scene& parent_scene_;
  absl::flat_hash_map<std::string, std::reference_wrapper<Object>> objects_;
  std::shared_ptr<std::queue<Message>> message_queue_;
  std::string name_;
};

// TODO: implement
class ObjectGroup {};

class Object : public Renderable {
 public:
  const std::string& name() const& { return name_; }

  virtual void render() override {}

  const SDL_Rect& renderRect() const& { return render_rect; }

  void setPoint(int x, int y) {
    render_rect.x = x;
    render_rect.y = y;
  }
  void setWidth(int width) { render_rect.w = width; }
  void setHeight(int height) { render_rect.h = height; }

  std::forward_list<std::function<void(SDL_Event&)>>
  eventCallbacksWithDescriptor() {
    return callback_with_event_descriptor_;
  }
  std::forward_list<std::function<void()>> eventCallbacks() {
    return callback_;
  }

 protected:
  explicit Object(TruffleBehavior& parent_behavior, const Renderer& renderer,
                  std::string name)
      : Renderable(renderer), parent_behavior(parent_behavior), name_(name) {}

  /**
   * TODO: この方法は嫌なので別のやりかたを考える
   * @param callback
   */
  void setEventCallback(std::function<void()> callback) {
    callback_.push_front(callback);
  }
  void setEventCallback(std::function<void(SDL_Event&)> callback) {
    callback_with_event_descriptor_.push_front(callback);
  }

  template <class T>
  void sendMessage(std::string dst_behavior, T&& message) {
    parent_behavior.sendMessage(dst_behavior, message);
  }

 private:
  TruffleBehavior& parent_behavior;
  std::string name_;
  SDL_Rect render_rect;
  std::forward_list<std::function<void()>> callback_;
  std::forward_list<std::function<void(SDL_Event&)>>
      callback_with_event_descriptor_;
};

TruffleBehavior::TruffleBehavior(Scene& parent_scene, std::string name)
    : parent_scene_(parent_scene), name_(name) {
  parent_scene_.setBehavior(*this);
}

void TruffleBehavior::addObject(Object& object) {
  if (objects_.find(object.name()) != objects_.end()) {
    throw TruffleException("Duplicated name object can't be registered");
  }
  objects_.emplace(object.name(), object);
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
