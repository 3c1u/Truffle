//
// Created by shikugawa on 2020/12/26.
//

#ifndef TRUFFLE_EVENT_CALLBACK_H
#define TRUFFLE_EVENT_CALLBACK_H

#include <SDL2/SDL.h>

namespace Truffle {

class InterruptionEventCallback {
 public:
  virtual ~InterruptionEventCallback() = default;

  virtual void onKeyPressed(SDL_Event& ev) {}

  virtual void onMouseButtonPressed(SDL_Event& ev) {}
};
}  // namespace Truffle

#endif  // Truffle_EVENT_CALLBACK_H
