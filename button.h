//
// Created by shikugawa on 2020/12/27.
//

#ifndef TRUFFLE_BUTTON_H
#define TRUFFLE_BUTTON_H

#include <SDL2/SDL.h>

#include "behavior.h"
#include "renderable.h"
#include "renderer.h"

namespace Truffle {

using MouseClickActionCallback = std::function<void()>;
using Color = SDL_Color;

class FixedButton : public FixedRenderable, public TruffleBehavior {
 public:
  explicit FixedButton(Renderer& renderer, std::string name, int x, int y,
                       int width, int height, Color color,
                       MouseClickActionCallback click_cb)
      : FixedRenderable(renderer, name),
        TruffleBehavior(name),
        x_(x),
        y_(y),
        width_(width),
        height_(height),
        color_(color),
        click_cb_(click_cb) {}

  // FixedRenderable
  void render() override {
    SDL_Rect fill_rect = {x_, y_, width_, height_};
    SDL_SetRenderDrawColor(renderer_.entity(), color_.r, color_.g, color_.b,
                           color_.a);
    SDL_RenderFillRect(renderer_.entity(), &fill_rect);
  }

  // TruffleBehavior
  void update() override { render(); }

  void onMouseButtonPressed(SDL_Event& ev) override {
    if (ev.button.button != SDL_BUTTON_LEFT) {
      return;
    }
    int x, y;
    SDL_GetMouseState(&x, &y);
    if ((x_ <= x && x <= x_ + width_) && (y_ <= y && y <= y_ + height_)) {
      click_cb_();
    }
  }

 private:
  int x_;
  int y_;
  int width_;
  int height_;
  Color color_;
  MouseClickActionCallback click_cb_;
};

}  // namespace Truffle

#endif  // Truffle_BUTTON_H
