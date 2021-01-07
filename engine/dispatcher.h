/**
 * @file      dispatcher.h
 * @author    Rei Shimizu (shikugawa) <shikugawa@gmail.com>
 * @brief     Main event loop and rendering frame
 *
 * @copyright Copyright 2021 Rei Shimizu. All rights reserved.
 */

#ifndef TRUFFLE_DISPATCHER_H
#define TRUFFLE_DISPATCHER_H

#include <SDL2/SDL.h>

#include <functional>
#include <iostream>

#include "common/non_copyable.h"
#include "common/singleton.h"
#include "context.h"
#include "controller/fps.h"
#include "event.h"
#include "metrics.h"
#include "scene_manager.h"
#include "wrapper/sdl2/renderer_storage.h"

namespace Truffle {

template <class SceneState>
class Dispatcher : public MutableSingleton<Dispatcher<SceneState>>,
                   NonCopyable {
 public:
  void run();

 private:
  friend class MutableSingleton<Dispatcher<SceneState>>;

  Dispatcher(SceneManager<SceneState>& m, bool enable_fps_calc = false)
      : scene_manager_(m),
        exit_handler_([](Event&) {}),
        enable_fps_calc_(enable_fps_calc),
        fps_controller_("fps") {
    if (enable_fps_calc_) {
      FpsMetrics::get();
      Context::get().setController(fps_controller_);
    }
  }

  Dispatcher(SceneManager<SceneState>& m,
             CustomEventCallback dispatcher_exit_callback,
             bool enable_fps_calc = false)
      : scene_manager_(m),
        exit_handler_(std::move(dispatcher_exit_callback)),
        enable_fps_calc_(enable_fps_calc),
        fps_controller_("fps") {
    if (enable_fps_calc_) {
      FpsMetrics::get();
      Context::get().setController(fps_controller_);
    }
  }

  bool handleEvents();

  CustomEventCallback exit_handler_;
  SceneManager<SceneState>& scene_manager_;
  FpsController fps_controller_;
  bool enable_fps_calc_ = false;
};

template <class SceneState>
void Dispatcher<SceneState>::run() {
  // Call startup functions on root scene
  scene_manager_.currentScene().initScene();

  while (true) {
    // Handle Event
    if (!handleEvents()) {
      return;
    }

    SDL_SetRenderDrawColor(
        const_cast<SDL_Renderer*>(
            RendererStorage::get().activeRenderer()->entity()),
        0xff, 0xff, 0xff, 0xff);
    SDL_RenderClear(const_cast<SDL_Renderer*>(
        RendererStorage::get().activeRenderer()->entity()));

    // Controllers
    for (auto& [_, controller] : scene_manager_.currentScene().controllers()) {
      for (auto& [_, object] : controller.get().visibleObjects()) {
        object.get().render();
      }
    }

    // TODO: render global controllers

    SDL_RenderPresent(const_cast<SDL_Renderer*>(
        RendererStorage::get().activeRenderer()->entity()));

    if (enable_fps_calc_) {
      FpsMetrics::get().incFrame();

      if (FpsMetrics::get().shouldCalcFps()) {
        fps_controller_.setFps(FpsMetrics::get().fps());
      }
    }
  }
}

template <class SceneState>
bool Dispatcher<SceneState>::handleEvents() {
  Event e;
  while (SDL_PollEvent(&e) != 0) {
    if (e.type == SDL_QUIT) {
      exit_handler_(e);
      return false;
    }
    if (e.user.type == EV_SCENE_CHANGED) {
      scene_manager_.transitScene();
    }
    // Handle controller update
    for (auto& [_, controller] : scene_manager_.currentScene().controllers()) {
      controller.get().update(e);
    }

    // Handle events related with hardware interruption
    for (const auto& [_, controller] :
         scene_manager_.currentScene().controllers()) {
      for (const auto& [_, object] : controller.get().visibleObjects()) {
        for (const auto& callback : object.get().eventCallbacks()) {
          callback(e);
        }
      }
      for (const auto& [_, object] : controller.get().invisibleObjects()) {
        for (const auto& callback : object.get().eventCallbacks()) {
          callback(e);
        }
      }
    }
  }
  return true;
}

}  // namespace Truffle

#endif  // TRUFFLE_DISPATCHER_H
