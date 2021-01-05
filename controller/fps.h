/**
 * @file      fps.h
 * @author    Rei Shimizu (shikugawa) <shikugawa@gmail.com>
 * @brief
 *
 * @copyright Copyright 2021 Rei Shimizu. All rights reserved.
 */

#ifndef TRUFFLE_FPS_H
#define TRUFFLE_FPS_H

#include "wrapper/sdl2/event.h"

namespace Truffle {

class FpsController : public SceneIsolatedTruffleControllerImpl {
 public:
  explicit FpsController(std::string name)
      : SceneIsolatedTruffleControllerImpl(name),
        texture_(*this, renderer, font, name + "_texture", 0, 0) {
    addObject(texture_);
  }

  void update(SDL_Event& ev) final {
    Color cg{0x00, 0x00, 0x00, 0xff};
    texture_.loadBlendTexture("string", cg);
  }

 private:
  TextTexture texture_;
};

}  // namespace Truffle

#endif  // TRUFFLE_FPS_H
