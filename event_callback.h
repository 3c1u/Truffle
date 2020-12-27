//
// Created by shikugawa on 2020/12/26.
//

#ifndef MIRAI_EVENT_CALLBACK_H
#define MIRAI_EVENT_CALLBACK_H

#include <SDL2/SDL.h>

namespace Mirai {
class KeyPressCallback {
 public:
  virtual ~KeyPressCallback() = default;

  virtual void onKeyPressed(SDL_Event& ev) {}

  virtual void onMouseButtonPressed(SDL_Event& ev) {}
};
}  // namespace Mirai

#endif  // MIRAI_EVENT_CALLBACK_H
