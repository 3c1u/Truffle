//
// Created by shikugawa on 2020/12/27.
//

#ifndef TRUFFLE_BUTTON_H
#define TRUFFLE_BUTTON_H

#include <SDL2/SDL.h>

#include "renderer.h"
#include "texture.h"

namespace Truffle {

using Color = SDL_Color;

/**
 * 基礎的なボタンの機能を提供するクラス
 *
 * example:
 *
 * class HogeButton : public Button {
 * public:
 *   HogeButton(Renderer& renderer, std::string name, int x, int y, int width,
 * int height, Color color) : Button(renderer, name, x, y, width, height, color)
 * {}
 *
 *   void onHover() override {
 *       std::cout << "hover" << std::endl;
 *   }
 *
 *   void onClicked(SDL_Event& ev) override {
 *       std::cout << "clicked" << std::endl;
 *   }
 * };
 */
class Button : public FixedRenderable {
 public:
  explicit Button(Renderer& renderer, std::string name, int x, int y, int width,
                  int height, Color color)
      : FixedRenderable(renderer, name),
        x_(x),
        y_(y),
        width_(width),
        height_(height),
        color_(color) {}

  // FixedRenderable
  void render() override {
    SDL_Rect fill_rect = {x_, y_, width_, height_};
    SDL_SetRenderDrawColor(renderer_.entity(), color_.r, color_.g, color_.b,
                           color_.a);
    SDL_RenderFillRect(renderer_.entity(), &fill_rect);
  }

  virtual void onHover() {}

  virtual void onClicked(SDL_Event& ev) {}

  bool onMouseHovered() {
    int x, y;
    SDL_GetMouseState(&x, &y);
    if ((x_ <= x && x <= x_ + width_) && (y_ <= y && y <= y_ + height_)) {
      return true;
    }
    return false;
  }

  bool onMouseLeftButtonPressed(SDL_Event& ev) {
    if (ev.button.button != SDL_BUTTON_LEFT) {
      return false;
    }
    return onMouseHovered();
  }

 private:
  int x_;
  int y_;
  int width_;
  int height_;
  Color color_;
};

class ImageButton : public Button {
 public:
};

}  // namespace Truffle

#endif  // Truffle_BUTTON_H
