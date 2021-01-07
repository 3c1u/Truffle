/**
 * @file      engine.h
 * @author    Rei Shimizu (shikugawa) <shikugawa@gmail.com>
 * @brief     Root initializer of Truffle engine
 *
 * @copyright Copyright 2021 Rei Shimizu. All rights reserved.
 */

#ifndef TRUFFLE_ENGINE_H
#define TRUFFLE_ENGINE_H

#include <SDL2/SDL_Image.h>

#include <memory>

#include "dispatcher.h"
#include "engine_config.h"
#include "scene_manager.h"
#include "wrapper/sdl2/font.h"
#include "wrapper/sdl2/font_storage.h"
#include "wrapper/sdl2/renderer.h"
#include "wrapper/sdl2/renderer_storage.h"
#include "wrapper/sdl2/window.h"

namespace Truffle {

template <class SceneState>
class Engine {
 public:
  Engine(EngineConfig& config);
  ~Engine();

  /**
   * 新しいシーンを作成する。
   * @param state
   * @param scene_name
   * @return
   */
  TruffleScene& newScene(SceneState state, std::string scene_name);

  /**
   * シーン遷移を定義する。
   * @param from
   * @param to
   */
  void setSceneTransition(SceneState from, SceneState to);

  /**
   * イベントディスパッチャーを起動する
   */
  void start();

 private:
  std::unique_ptr<SceneManager<SceneState>> scene_manager_;
  std::unique_ptr<Dispatcher<SceneState>> dispatcher_;
};

template <class SceneState>
Engine<SceneState>::Engine(EngineConfig& config) {
  if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
    throw TruffleException("Failed to init engine");
  }

  const auto& window_tmp =
      Window::get(config.name, config.window_width, config.window_height);

  auto& renderer_storage_tmp = RendererStorage::get();
  renderer_storage_tmp.activateRenderer(window_tmp);
  renderer_storage_tmp.activeRenderer()->setDrawColor(config.renderer_color);

  auto& font_storage_tmp = FontStorage::get();
  if (!config.font_paths.empty()) {
    for (const auto& [font_name, path] : config.font_paths) {
      font_storage_tmp.loadFont(font_name, path);
    }
  }

  scene_manager_ = std::make_unique<SceneManager<SceneState>>();
  auto& dispatcher_tmp =
      Dispatcher<SceneState>::get(*scene_manager_, config.debug_fps);
  dispatcher_ = std::unique_ptr<Dispatcher<SceneState>>(&dispatcher_tmp);
}

template <class SceneState>
Engine<SceneState>::~Engine() {
  SDL_Quit();
}

template <class SceneState>
TruffleScene& Engine<SceneState>::newScene(SceneState state,
                                           std::string scene_name) {
  assert(scene_manager_ != nullptr);
  return scene_manager_->addScene(state, scene_name);
}

template <class SceneState>
void Engine<SceneState>::setSceneTransition(SceneState from, SceneState to) {
  assert(scene_manager_ != nullptr);
  scene_manager_->setSceneTransition(from, to);
}

template <class SceneState>
void Engine<SceneState>::start() {
  assert(dispatcher_ != nullptr);
  dispatcher_->run();
}

}  // namespace Truffle

#endif  // TRUFFLE_ENGINE_H
