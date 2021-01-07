/**
 * @file      fps.h
 * @author    Rei Shimizu (shikugawa) <shikugawa@gmail.com>
 * @brief
 *
 * @copyright Copyright 2021 Rei Shimizu. All rights reserved.
 */

#ifndef TRUFFLE_FPS_H
#define TRUFFLE_FPS_H

#include "engine/controller.h"
#include "object/text.h"

namespace Truffle {

class FpsController : public TruffleController {
 public:
  explicit FpsController(std::string name)
      : TruffleController(name),
        text_(name + "_text", std::to_string(fps_) + " fps", 0, 0,
              Color{0xff, 0xff, 0xff, 0xff}, "lazy", 20) {
    appendObject(text_);
  }

  void update(SDL_Event& ev) final {
    text_.setText(std::to_string(fps_) + " fps");
  }

  void setFps(double fps) { fps_ = fps; }

 private:
  SolidText text_;
  double fps_ = 0;
};

}  // namespace Truffle

#endif  // TRUFFLE_FPS_H
