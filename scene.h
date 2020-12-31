//
// Created by shikugawa on 2020/12/26.
//

#ifndef TRUFFLE_SCENE_H
#define TRUFFLE_SCENE_H

#include <absl/container/flat_hash_map.h>

#include <memory>
#include <mutex>
#include <queue>
#include <string>

#include "behavior.h"
#include "button.h"
#include "logger.h"
#include "non_copyable.h"
#include "renderable.h"
#include "stateful_object_manager.h"

namespace Truffle {

class Scene : NonCopyable {
 public:
  using ScenePtr = std::shared_ptr<Scene>;

  Scene(std::string scene_name) : name_(scene_name) {}

  void initScene() const&;
  void setBehavior(TruffleBehavior& b);
  void setButton(ButtonBase& b);

  const std::string& name() const& { return name_; }
  const absl::flat_hash_map<std::string,
                            std::reference_wrapper<TruffleBehavior>>&
  behaviors() const& {
    return behaviors_;
  }
  const absl::flat_hash_map<std::string, std::reference_wrapper<ButtonBase>>&
  buttons() const& {
    return buttons_;
  }

 private:
  std::string name_;

  absl::flat_hash_map<std::string, std::reference_wrapper<TruffleBehavior>>
      behaviors_;
  absl::flat_hash_map<std::string, std::reference_wrapper<ButtonBase>> buttons_;
};

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

template <class SceneState>
class SceneManager : NonCopyable {
 public:
  /**
   * シーンの状態を登録する。生成されたシーンの参照を返す。
   *
   * @param state
   * @param scene_name
   * @return
   */
  Scene& addScene(SceneState state, std::string scene_name) {
    if (state_manager_.initialized()) {
      state_manager_.bindStatefulObject(state, scene_name);
    } else {
      state_manager_.setInitStatefulObject(state, scene_name);
    }
    return state_manager_.statefulObject(state);
  }

  /**
   * シーンの遷移を定義する。与えられたシーンが未作成であれば例外を返す。
   *
   * @param from
   * @param to
   */
  void setSceneTransition(SceneState from, SceneState to) {
    try {
      state_manager_.statefulObject(from);
      state_manager_.statefulObject(to);
    } catch (TruffleException&) {
      throw TruffleException("Provided scene is not registered");
    }
    state_manager_.setStateTransition(from, to);
    state_manager_.setStateTransition(to, from);
  }

  /**
   * シーン遷移イベントを発行する
   *
   * @param dst_scene 遷移先
   */
  void sendSceneTransitionSignal(SceneState dst_scene) {
    // SDL_PushEventはスレッドセーフだがstd::queueはスレッドセーフではないので、
    // シーン遷移キューとイベントキューの不整合を防ぐためにロックを獲る
    std::unique_lock<std::mutex> lock(mux_);
    pending_scene_transition_.push(dst_scene);
    SDL_Event transition_event;
    transition_event.type = SDL_USEREVENT;
    transition_event.user.type = EV_SCENE_CHANGED;
    if (SDL_PushEvent(&transition_event) < 0) {
      log(LogLevel::ERROR, "Failed to push event to event diapatcher");
      pending_scene_transition_.pop();
    }
  }

  /**
   * 実際にシーン遷移を行う。pending
   * queueに遷移先のイベントが格納されている必要がある。
   */
  void transitScene() {
    if (pending_scene_transition_.empty()) {
      log(LogLevel::WARN,
          "Can't invoke scene transition with empty pending queue");
      return;
    }
    std::unique_lock<std::mutex> lock(mux_);
    auto to = pending_scene_transition_.front();
    pending_scene_transition_.pop();
    state_manager_.stateTransition(to);
  }

  /**
   * 現在アクティブなシーンを返す
   *
   * @return
   */
  const Scene& currentScene() { return state_manager_.activeStateObject(); }

  /**
   * 現在のシーンの状態を返す
   *
   * @return
   */
  SceneState currentSceneState() { return state_manager_.activeState(); }

 private:
  std::queue<SceneState> pending_scene_transition_;
  StatefulObjectManager<Scene, SceneState> state_manager_;
  std::mutex mux_;
};

}  // namespace Truffle

#endif  // Truffle_SCENE_H
