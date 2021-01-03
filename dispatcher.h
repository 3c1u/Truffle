//
// Created by shikugawa on 2020/12/19.
//

#ifndef TRUFFLE_DISPATCHER_H
#define TRUFFLE_DISPATCHER_H

#include <SDL2/SDL.h>
#include <absl/container/flat_hash_map.h>

#include <cassert>
#include <forward_list>
#include <functional>

#include "fps.h"
#include "scene_manager.h"
#include "texture.h"

namespace Truffle {

template <class SceneState>
class Dispatcher : NonCopyable {
 public:
  using Callback = std::function<void(SDL_Event&)>;

  Dispatcher(SceneManager<SceneState>& m, Renderer& r,
             bool enable_fps_calc = false)
      : scene_manager_(m),
        renderer_(r),
        exit_handler_([](SDL_Event&) {}),
        enable_fps_calc_(enable_fps_calc) {
    if (enable_fps_calc_) {
//      scene_manager_.setSceneIsolatedController();
    }
  }

  Dispatcher(SceneManager<SceneState>& m, Renderer& r,
             Callback dispatcher_exit_callback, bool enable_fps_calc = false)
      : scene_manager_(m),
        renderer_(r),
        exit_handler_(std::move(dispatcher_exit_callback)),
        enable_fps_calc_(enable_fps_calc) {}

  void run() {
    // Call startup functions on root scene
    scene_manager_.currentScene().initScene();

    while (true) {
      // Handle SDL_Event
      if (!handleEvents()) {
        return;
      }

      SDL_SetRenderDrawColor(const_cast<SDL_Renderer*>(renderer_.entity()),
                             0xff, 0xff, 0xff, 0xff);
      SDL_RenderClear(const_cast<SDL_Renderer*>(renderer_.entity()));

      // Controllers
      for (auto& [_, controller] :
           scene_manager_.currentScene().controllers()) {
        for (auto& [_, object] : controller.get().targetObjects()) {
          object.get().render();
        }
      }

      SDL_RenderPresent(const_cast<SDL_Renderer*>(renderer_.entity()));

      // Calculate Fps
      if (enable_fps_calc_) {
        ++frame_;
      }
    }
  }

 private:
  bool handleEvents() {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
      if (e.type == SDL_QUIT) {
        exit_handler_(e);
        return false;
      }
      if (e.user.type == EV_SCENE_CHANGED) {
        scene_manager_.transitScene();
      }
      // Handle controller update
      for (auto& [_, controller] :
           scene_manager_.currentScene().controllers()) {
        controller.get().update(e);
      }
      // Handle events related with hardware interruption
      for (const auto& [_, controller] :
           scene_manager_.currentScene().controllers()) {
        for (const auto& [_, object] : controller.get().targetObjects()) {
          for (const auto& callback : object.get().eventCallbacks()) {
            callback(e);
          }
        }
      }
    }
    return true;
  }

  Callback exit_handler_;
  SceneManager<SceneState>& scene_manager_;
  Renderer& renderer_;
  bool enable_fps_calc_ = false;
  uint64_t frame_;

//  FpsController fps_controller_()
};

}  // namespace Truffle

#endif  // Truffle_DISPATCHER_H
