//
// Created by shikugawa on 2020/12/26.
//

#ifndef TRUFFLE_OBJECT_TREE_H
#define TRUFFLE_OBJECT_TREE_H

#include <absl/container/flat_hash_map.h>

#include <forward_list>
#include <memory>
#include <queue>
#include <variant>

#include "bus.h"
#include "non_copyable.h"
#include "renderable.h"

namespace Truffle {

class TruffleController;
class TruffleObject;

class TruffleScene : NonCopyable {
 public:
  explicit TruffleScene(std::string scene_name);

  /**
   * シーンの初期化を行う。
   */
  void initScene() const&;

  /**
   * 同一シーン内のコントローラーにメッセージを送る。メッセージの送信に失敗した場合は例外を送出する。
   * @tparam T
   * @param dst_controller
   * @param message
   * @return
   */
  template <class T>
  bool sendMessage(std::string dst_controller, T&& message) {
    return bus_.sendMessage(dst_controller, std::forward<T&&>(message));
  }

  [[nodiscard]] const std::string& name() const& { return name_; }
  [[nodiscard]] const absl::flat_hash_map<
      std::string, std::reference_wrapper<TruffleController>>&
  controllers() const& {
    return controllers_;
  }

 private:
  friend class TruffleControllerImpl;

  /**
   * コントローラーを追加する。
   * @param controller
   */
  void setController(TruffleController& controller);

  std::string name_;

  EventMessageBus& bus_;
  absl::flat_hash_map<std::string, std::reference_wrapper<TruffleController>>
      controllers_;
};

class TruffleController : NonCopyable {
 public:
  /**
   * コントローラーのコンストラクタ
   * @param name
   * コントローラーの名前。名前に重複があると例外が発生するので注意。
   */
  TruffleController(std::string name);

  virtual ~TruffleController() = default;

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
  void addObject(TruffleObject& object);

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
   * このコントローラーによって管理されているすべてのオブジェクトを返す。
   * @return
   */
  [[nodiscard]] const absl::flat_hash_map<
      std::string, std::reference_wrapper<TruffleObject>>&
  targetObjects() const& {
    return objects_;
  }
  absl::flat_hash_map<std::string, std::reference_wrapper<TruffleObject>>&
  targetObjects() & {
    return objects_;
  }

  [[nodiscard]] const std::string& name() const& { return name_; }

  /**
   * メッセージを1件送る
   * @param dst_controller
   * @param message
   */
  virtual void sendMessage(std::string dst_controller, Message&& message) = 0;
  virtual void sendMessage(std::string dst_controller,
                           const Message& message) = 0;

 private:
  absl::flat_hash_map<std::string, std::reference_wrapper<TruffleObject>>
      objects_;
  std::shared_ptr<std::queue<Message>> message_queue_;
  std::string name_;
};

/**
 * 普通のコントローラー
 */
class TruffleControllerImpl : public TruffleController {
 public:
  /**
   * コントローラーのコンストラクタ
   * @param parent_scene コントローラーが所属するシーンの参照
   * @param name
   * コントローラーの名前。名前に重複があると例外が発生するので注意。
   */
  TruffleControllerImpl(TruffleScene& parent_scene, std::string name);

  void sendMessage(std::string dst_controller, Message&& message) override {
    parent_scene_.sendMessage(dst_controller, std::forward<Message&&>(message));
  }
  void sendMessage(std::string dst_controller,
                   const Message& message) override {
    parent_scene_.sendMessage(dst_controller,
                              std::forward<const Message&>(message));
  }

 private:
  TruffleScene& parent_scene_;
};

class SceneIsolatedTruffleControllerImpl : public TruffleController {
 public:
  /**
   * コントローラーのコンストラクタ
   * @param parent_scene コントローラーが所属するシーンの参照
   * @param name
   * コントローラーの名前。名前に重複があると例外が発生するので注意。
   */
  SceneIsolatedTruffleControllerImpl(std::string name)
      : TruffleController(name) {}

  void sendMessage(std::string dst_controller, Message&& message) override {
    // unimplemented
    assert(false);
  }
  void sendMessage(std::string dst_controller,
                   const Message& message) override {
    // unimplemented
    assert(false);
  }
};

// TODO: implement
class ObjectGroup {};

class TruffleObject : public Renderable, NonCopyable {
 public:
  virtual void render() override {}

  const std::string& name() const& { return name_; }

  const SDL_Rect& renderRect() const& { return render_rect; }

  void setPoint(int x, int y) {
    render_rect.x = x;
    render_rect.y = y;
  }
  void setWidth(int width) { render_rect.w = width; }
  void setHeight(int height) { render_rect.h = height; }

  std::forward_list<std::function<void(SDL_Event&)>> eventCallbacks() {
    return callback_;
  }

 protected:
  explicit TruffleObject(TruffleController& parent_controller,
                         const Renderer& renderer, std::string name)
      : Renderable(renderer),
        parent_controller_(parent_controller),
        name_(name) {}

  /**
   * イベントハンドラーを登録する。
   * @param callback
   */
  void setEventCallback(std::function<void(SDL_Event&)> callback) {
    callback_.push_front(callback);
  }

  template <class T>
  void sendMessage(std::string dst_controller, T&& message) {
    parent_controller_.sendMessage(dst_controller, std::forward<T&&>(message));
  }

 private:
  TruffleController& parent_controller_;

  std::string name_;
  SDL_Rect render_rect;
  std::forward_list<std::function<void(SDL_Event&)>> callback_;
};

TruffleController::TruffleController(std::string name) : name_(name) {}

void TruffleController::addObject(TruffleObject& object) {
  if (objects_.find(object.name()) != objects_.end()) {
    throw TruffleException("Duplicated name object can't be registered");
  }
  objects_.emplace(object.name(), object);
}

std::optional<Message> TruffleController::recvMessage() {
  assert(message_queue_ != nullptr);
  if (message_queue_->empty()) {
    return std::nullopt;
  }
  auto message = message_queue_->front();
  message_queue_->pop();
  return message;
}

TruffleControllerImpl::TruffleControllerImpl(TruffleScene& parent_scene,
                                             std::string name)
    : TruffleController(name), parent_scene_(parent_scene) {
  parent_scene_.setController(*this);
}

TruffleScene::TruffleScene(std::string scene_name)
    : name_(std::move(scene_name)), bus_(EventMessageBus::get()) {}

void TruffleScene::initScene() const& {
  for (const auto& [_, cb] : controllers_) {
    cb.get().start();
  }
}

void TruffleScene::setController(TruffleController& controller) {
  if (controllers_.find(controller.name()) != controllers_.end()) {
    throw TruffleException(absl::StrFormat(
        "controller %s had already registered", controller.name()));
  }
  log(LogLevel::INFO, absl::StrFormat("controller %s registered to scene %s",
                                      controller.name(), name_));
  auto queue = bus_.getMessageQueue(controller.name());
  controller.setMessageQueue(queue);
  controllers_.emplace(controller.name(), controller);
}

}  // namespace Truffle

#endif  // Truffle_BEHAVIOR_H
