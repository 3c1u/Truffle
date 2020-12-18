//
// Created by shikugawa on 2020/12/19.
//

#ifndef MIRAI_DISPATCHER_H
#define MIRAI_DISPATCHER_H

#include <SDL2/SDL.h>
#include <absl/container/flat_hash_map.h>

#include <forward_list>
#include <functional>

#include "logger.h"
#include "renderable.h"

namespace Mirai {

using Callback = std::function<int()>;

class Dispatcher {
 public:
  Dispatcher() {
    callbacks_[SDL_QUIT] = []() -> int { return -1; };
  }
  Dispatcher(Callback dispatcher_exit_callback) {
    callbacks_[SDL_QUIT] = [&dispatcher_exit_callback]() -> int {
      dispatcher_exit_callback();
      return -1;
    };
  }

  void run() {
    while (true) {
      SDL_Event e;
      if (handleEvent(&e) < 0) {
        log(LogLevel::INFO, "Event dispatcher stopped.");
        return;
      }

      for (const auto& r : renderables_) {
        r->render();
      }
    }
  }

  void addCallback(Uint32 type, Callback cb) {
    if (type == SDL_QUIT) {
      log(LogLevel::WARN, "Not allowed to rewrite SDL_QUIT event callback.");
      return;
    }
    callbacks_[type] = cb;
  }

 private:
  int handleEvent(SDL_Event* e) {
    while (SDL_PollEvent(e) != 0) {
      for (const auto& [type, cb] : callbacks_) {
        if (e->type != type) {
          continue;
        }
        return cb();
      }
    }
    return 0;
  }

  absl::flat_hash_map<Uint32, Callback> callbacks_;
  std::forward_list<RenderablePtr> renderables_;
};
}  // namespace Mirai

#endif  // Mirai_DISPATCHER_H
