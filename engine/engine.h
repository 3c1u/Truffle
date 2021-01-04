/**
 * @file      engine.h
 * @author    Rei Shimizu (shikugawa) <shikugawa@gmail.com>
 * @brief     Root initializer of Truffle engine
 *
 * @copyright Copyright 2021 Rei Shimizu. All rights reserved.
 */

#ifndef TRUFFLE_ENGINE_H
#define TRUFFLE_ENGINE_H

#include <memory>

#include "SDL_image.h"
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

 private:
  std::unique_ptr<Window> window_;
  std::unique_ptr<SceneManager<SceneState>> scene_manager_;
  RendererStorage renderer_storage_;
  FontStorage font_storage_;
};

template <class TruffleScene>
Engine<TruffleScene>::Engine(EngineConfig& config) {
  if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
    throw TruffleException("Failed to init engine");
  }

  const auto& window_tmp =
      Window::get(config.name, config.window_width, config.window_height);
  window_ = std::unique_ptr<Window>(const_cast<Window*>(&window_tmp));

  renderer_storage_.activateRenderer(*window_);
  renderer_storage_.activeRenderer()->setDrawColor(config.renderer_color);

  if (!config.font_paths.empty()) {
    for (const auto& [font_name, path] : config.font_paths) {
      font_storage_.loadFont(font_name, path);
    }
  }
}

template <class SceneState>
Engine<SceneState>::~Engine() {
  SDL_Quit();
}

template <class SceneState>
TruffleScene& Engine<SceneState>::newScene(SceneState state,
                                           std::string scene_name) {
  return scene_manager_->addScene(state, scene_name);
}

}  // namespace Truffle

#endif  // TRUFFLE_ENGINE_H
