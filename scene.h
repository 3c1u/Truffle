//
// Created by shikugawa on 2020/12/26.
//

#ifndef TRUFFLE_SCENE_H
#define TRUFFLE_SCENE_H

#include <absl/container/flat_hash_map.h>

#include <memory>
#include <mutex>
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

  void initScene() const& {
    for (auto& cb : behaviors_) {
      cb.get().start();
    }
  }

  const std::string& sceneName() { return name_; }

  void setBehavior(TruffleBehavior& b) {
    log(LogLevel::INFO, absl::StrFormat("behavior %s registered to scene %s",
                                        b.behaviorName(), name_));
    behaviors_.push_front(b);
  }

  void setButton(ButtonBase& b) {
    log(LogLevel::INFO, absl::StrFormat("button %s registered to scene %s",
                                        b.buttonName(), name_));
    buttons_.push_front(b);
  }

  const std::forward_list<std::reference_wrapper<TruffleBehavior>>& behaviors()
      const& {
    return behaviors_;
  }

  const std::forward_list<std::reference_wrapper<ButtonBase>>& buttons()
      const& {
    return buttons_;
  }

 private:
  std::string name_;

  std::forward_list<std::reference_wrapper<TruffleBehavior>> behaviors_;
  std::forward_list<std::reference_wrapper<ButtonBase>> buttons_;
};

using ScenePtr = Scene::ScenePtr;

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
      state_manager_.bindStatefulObject(state,
                                        std::make_shared<Scene>(scene_name));
    } else {
      state_manager_.setInitStatefulObject(state,
                                           std::make_shared<Scene>(scene_name));
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
    state_manager_.bindStatefulObject(from, to);
    state_manager_.bindStatefulObject(to, from);
  }

  /**
   * シーン遷移を行う
   *
   * @param to
   * @return
   */
  bool transitScene(SceneState to) { state_manager_.stateTransition(to); }

  /**
   * 現在アクティブなシーンを返す
   *
   * @return
   */
  const Scene& currentScene() { return state_manager_.activeStateObject(); }

 private:
  StatefulObjectManager<Scene, SceneState> state_manager_;
};

}  // namespace Truffle

#endif  // Truffle_SCENE_H
