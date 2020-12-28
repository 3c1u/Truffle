//
// Created by shikugawa on 2020/12/27.
//

#ifndef TRUFFLE_BUTTON_H
#define TRUFFLE_BUTTON_H

#include <SDL2/SDL.h>

#include <cassert>
#include <set>
#include <unordered_map>

#include "exception.h"
#include "logger.h"
#include "stateful_object_manager.h"
#include "texture.h"

namespace Truffle {

using Color = SDL_Color;

class ButtonEventCallback {
 public:
  virtual ~ButtonEventCallback() = default;

  /**
   * ボタンが押された時のコールバック
   */
  virtual void onButtonPressed(SDL_Event& ev) = 0;

  /**
   * ボタンがホバーされた時のコールバック
   */
  virtual void onMouseHovered() = 0;

  /**
   * ボタンがアンホバーされた時のコールバック
   */
  virtual void onMouseUnhovered() = 0;

 protected:
  bool isMouseHovered(int x, int y, int width, int height) {
    int mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);
    if ((x <= mouse_x && mouse_x <= x + width) &&
        (y <= mouse_y && mouse_y <= y + height)) {
      return true;
    }
    return false;
  }

  bool isMouseLeftButtonPressed(SDL_Event& ev, int x, int y, int width,
                                int height) {
    if (ev.button.button != SDL_BUTTON_LEFT) {
      return false;
    }
    return isMouseHovered(x, y, width, height);
  }
};

enum class ButtonState {
  Normal,
  Hovered,
  Pressed,
};

/**
 * 基礎的なボタンの機能を提供するクラス
 */
class ImageButton : public FixedRenderable, public ButtonEventCallback {
 public:
  ImageButton(Renderer& r, std::string name, int x, int y,
              std::string path_normal, std::string path_hovered = "",
              std::string path_pressed = "")
      : FixedRenderable(r, name), x_(x), y_(y) {
    state_manager_.setInitStatefulObject(
        ButtonState::Normal,
        std::make_shared<ImageTexture>(r, path_normal, name + "_normal"));
    // Bind object
    if (!path_hovered.empty()) {
      state_manager_.bindStatefulObject(
          ButtonState::Hovered,
          std::make_shared<ImageTexture>(r, path_hovered, name + "_hovered"));
    }
    if (!path_pressed.empty()) {
      state_manager_.bindStatefulObject(
          ButtonState::Pressed,
          std::make_shared<ImageTexture>(r, path_pressed, name + "_pressed"));
    }
    state_manager_.defineStateTransition(ButtonState::Hovered,
                                         ButtonState::Pressed);
    state_manager_.defineStateTransition(ButtonState::Normal,
                                         ButtonState::Hovered);
    state_manager_.defineStateTransition(ButtonState::Hovered,
                                         ButtonState::Normal);
  }

  const std::string& buttonName() { return name_; }

  // FixedRenderable
  void render() override {
    auto& active_state = state_manager_.activeStateObject();
    SDL_Rect render_rect = {x_, y_, active_state.width(),
                            active_state.height()};
    SDL_RenderCopy(renderer_.entity(), active_state.entity(),
                   nullptr /* TODO: introduce clip settings */, &render_rect);
  }

  // ButtonEventCallback
  virtual void onButtonPressed(SDL_Event& ev) override {
    auto& current_texture = state_manager_.activeStateObject();
    if (isMouseLeftButtonPressed(ev, x_, y_, current_texture.width(),
                                 current_texture.height()) &&
        state_manager_.activeState() == ButtonState::Hovered) {
      log(LogLevel::INFO, "State changed from Hovered to Pressed");
      state_manager_.stateTransition(ButtonState::Pressed);
    }
  }

  virtual void onMouseHovered() override {
    auto& current_texture = state_manager_.activeStateObject();
    if (isMouseHovered(x_, y_, current_texture.width(),
                       current_texture.height()) &&
        state_manager_.activeState() == ButtonState::Normal) {
      log(LogLevel::INFO, "State changed from Normal to Hovered");
      state_manager_.stateTransition(ButtonState::Hovered);
    }
  }

  virtual void onMouseUnhovered() override {
    auto& current_texture = state_manager_.activeStateObject();
    if (!isMouseHovered(x_, y_, current_texture.width(),
                        current_texture.height()) &&
        state_manager_.activeState() == ButtonState::Hovered) {
      log(LogLevel::INFO, "State changed from Hovered to Normal");
      state_manager_.stateTransition(ButtonState::Normal);
    }
  }

 private:
  StatefulObjectManager<ImageTexture, ButtonState> state_manager_;
  std::string name_;

  const int x_;
  const int y_;
};

}  // namespace Truffle

#endif  // Truffle_BUTTON_H
