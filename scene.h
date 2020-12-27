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
#include "logger.h"
#include "non_copyable.h"
#include "renderable.h"

namespace Truffle {
using Callback = std::function<void(SDL_Event& ev /* TODO: wrap SDL event */)>;

class Scene : NonCopyable {
 public:
  using ScenePtr = std::shared_ptr<Scene>;

  Scene(std::string name) : name_(name) {}

  ~Scene() { next_scenes_.clear(); }

  std::string& name() { return name_; }

  void setBehavior(TruffleBehavior& b) {
    log(LogLevel::INFO,
        absl::StrFormat("behavior %s registered to scene %s", b.name(), name_));
    behaviors_.push_front(b);
    callbacks_[SDL_KEYDOWN].push_back(
        [&b](SDL_Event& ev) { b.onKeyPressed(ev); });
    callbacks_[SDL_MOUSEBUTTONDOWN].push_back(
        [&b](SDL_Event& ev) { b.onMouseButtonPressed(ev); });
  }

  void addNextScene(ScenePtr next) { next_scenes_.emplace(next->name(), next); }

  const absl::flat_hash_map<Uint32, std::vector<Callback>>& callbacks() {
    return callbacks_;
  }

  const std::forward_list<std::reference_wrapper<TruffleBehavior>>&
  behaviors() {
    return behaviors_;
  }

  const absl::flat_hash_map<std::string, ScenePtr>& nextScenes() {
    return next_scenes_;
  }

 private:
  std::string name_;
  absl::flat_hash_map<std::string, ScenePtr> next_scenes_;
  absl::flat_hash_map<Uint32, std::vector<Callback>> callbacks_;
  std::forward_list<std::reference_wrapper<TruffleBehavior>> behaviors_;
};

using ScenePtr = Scene::ScenePtr;

class SceneManager : NonCopyable {
 public:
  SceneManager(ScenePtr root_scene) : current_scene_(root_scene) {}

  bool transitScene(std::string name) {
    assert(current_scene_);
    const auto& scene = current_scene_->nextScenes().find(name);
    if (scene != current_scene_->nextScenes().end()) {
      std::unique_lock l(mux_);
      current_scene_ = scene->second;
      log(LogLevel::INFO,
          absl::StrFormat("Scene transition to %s succeeded", name));
      return true;
    }
    log(LogLevel::WARN, absl::StrFormat("Missing candidate scene %s", name));
    return false;
  }

  ScenePtr currentScene() { return current_scene_; }

 private:
  std::mutex mux_;
  ScenePtr current_scene_;
};

}  // namespace Truffle

#endif  // Truffle_SCENE_H