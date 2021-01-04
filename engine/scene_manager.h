/**
 * @file      scene_manager.h
 * @author    Rei Shimizu (shikugawa) <shikugawa@gmail.com>
 * @brief     Handle scene management
 *
 * @copyright Copyright 2021 Rei Shimizu. All rights reserved.
 */

#ifndef TRUFFLE_SCENE_MANAGER_H
#define TRUFFLE_SCENE_MANAGER_H

#include <SDL2/SDL.h>
#include <absl/strings/str_format.h>

#include <mutex>
#include <queue>
#include <string>

#include "common/exception.h"
#include "common/logger.h"
#include "common/non_copyable.h"
#include "common/singleton.h"
#include "common/stateful_object_manager.h"
#include "scene.h"

namespace Truffle {

template <class SceneState>
class SceneManager : public MutableSingleton<SceneManager<SceneState>>,
                     NonCopyable {
 public:
  /**
   * シーンの状態を登録する。生成されたシーンの参照を返す。
   * @param state
   * @param scene_name
   * @return
   */
  TruffleScene& addScene(SceneState state, std::string scene_name);

  /**
   * シーンの遷移を定義する。与えられたシーンが未作成であれば例外を返す。
   * @param from
   * @param to
   */
  void setSceneTransition(SceneState from, SceneState to);

  /**
   * シーン遷移イベントを発行する
   * @param dst_scene 遷移先
   */
  void sendSceneTransitionSignal(SceneState dst_scene);

  /**
   * 実際にシーン遷移を行う。pending
   * queueに遷移先のイベントが格納されている必要がある。
   */
  void transitScene();

  /**
   * 現在アクティブなシーンを返す
   * @return
   */
  const TruffleScene& currentScene() {
    return state_manager_.activeStateObject();
  }

  /**
   * 現在のシーンの状態を返す
   * @return
   */
  SceneState currentSceneState() { return state_manager_.activeState(); }

 private:
  std::queue<SceneState> pending_scene_transition_;
  StatefulObjectManager<TruffleScene, SceneState> state_manager_;
  std::mutex mux_;
};

template <class SceneState>
TruffleScene& SceneManager<SceneState>::addScene(SceneState state,
                                                 std::string scene_name) {
  if (state_manager_.initialized()) {
    state_manager_.bindStatefulObject(state, scene_name);
  } else {
    state_manager_.setInitStatefulObject(state, scene_name);
  }
  return state_manager_.statefulObject(state);
}

template <class SceneState>
void SceneManager<SceneState>::setSceneTransition(SceneState from,
                                                  SceneState to) {
  try {
    state_manager_.statefulObject(from);
    state_manager_.statefulObject(to);
  } catch (TruffleException&) {
    throw TruffleException("Provided scene is not registered");
  }
  state_manager_.setStateTransition(from, to);
  state_manager_.setStateTransition(to, from);
}

template <class SceneState>
void SceneManager<SceneState>::sendSceneTransitionSignal(SceneState dst_scene) {
  // SDL_PushEventはスレッドセーフだがstd::queueはスレッドセーフではないので、
  // シーン遷移キューとイベントキューの不整合を防ぐためにロックを獲る
  std::unique_lock<std::mutex> lock(mux_);
  pending_scene_transition_.push(dst_scene);
  SDL_Event transition_event;
  transition_event.type = SDL_USEREVENT;
  transition_event.user.type = EV_SCENE_CHANGED;
  if (SDL_PushEvent(&transition_event) < 0) {
    // log_(LogLevel::ERROR, "Failed to push event to event diapatcher");
    pending_scene_transition_.pop();
  }
}

template <class SceneState>
void SceneManager<SceneState>::transitScene() {
  if (pending_scene_transition_.empty()) {
    // log_(LogLevel::WARN,
    //        "Can't invoke scene transition with empty pending queue");
    return;
  }
  std::unique_lock<std::mutex> lock(mux_);
  auto to = pending_scene_transition_.front();
  pending_scene_transition_.pop();
  state_manager_.stateTransition(to);
}

}  // namespace Truffle

#endif  // TRUFFLE_SCENE_MANAGER_H
