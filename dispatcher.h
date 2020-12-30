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

#include "scene.h"
#include "texture.h"

namespace Truffle {

class Dispatcher : NonCopyable {
 public:
  using Callback = std::function<void(SDL_Event&)>;

  Dispatcher(SceneManager& m, Renderer& r)
      : scene_manager_(m), renderer_(r), exit_handler_([](SDL_Event&) {}) {}

  Dispatcher(SceneManager& m, Renderer& r, Callback dispatcher_exit_callback)
      : scene_manager_(m),
        renderer_(r),
        exit_handler_(dispatcher_exit_callback) {}

  void run() {
    // Call startup functions on root scene
    assert(scene_manager_.currentScene());
    scene_manager_.currentScene()->initScene();

    while (true) {
      // Handle SDL_Event
      if (!handleEvents()) {
        return;
      }

      // Handle button callbacks
      assert(scene_manager_.currentScene());
      for (const auto& cb : scene_manager_.currentScene()->buttons()) {
        cb.get()._onMouseHovered();
        cb.get()._onMouseUnhovered();
      }

      SDL_SetRenderDrawColor(renderer_.entity(), 0xff, 0xff, 0xff, 0xff);
      SDL_RenderClear(renderer_.entity());

      // Render behaviors
      assert(scene_manager_.currentScene());
      for (auto& b : scene_manager_.currentScene()->behaviors()) {
        for (auto& r : b.get().targetRenderables()) {
          r.get().render();
        }
      }

      // Render buttons
      assert(scene_manager_.currentScene());
      for (auto& b : scene_manager_.currentScene()->buttons()) {
        b.get().render();
      }

      SDL_RenderPresent(renderer_.entity());
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

      // Handle behaviors update
      assert(scene_manager_.currentScene());
      for (auto& b : scene_manager_.currentScene()->behaviors()) {
        b.get().update(e);
      }

      // Handle button events related with hardware interruption
      assert(scene_manager_.currentScene());
      for (auto& b : scene_manager_.currentScene()->buttons()) {
        b.get()._onButtonPressed(e);
        b.get()._onButtonReleased(e);
      }
    }
    return true;
  }

  Callback exit_handler_;
  SceneManager& scene_manager_;
  Renderer& renderer_;
};
}  // namespace Truffle

#endif  // Truffle_DISPATCHER_H
