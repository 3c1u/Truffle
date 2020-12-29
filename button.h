//
// Created by shikugawa on 2020/12/27.
//

#ifndef TRUFFLE_BUTTON_H
#define TRUFFLE_BUTTON_H

#include <SDL2/SDL.h>

#include <cassert>
#include <set>
#include <unordered_map>

#include "event_publisher.h"
#include "exception.h"
#include "logger.h"
#include "stateful_object_manager.h"
#include "texture.h"

namespace Truffle {

class ButtonEventCallback {
 public:
  virtual ~ButtonEventCallback() = default;

  /**
   * ボタンが押された時のコールバック
   */
  virtual void onButtonPressed(SDL_Event& ev) = 0;

  /**
   * ボタンが離された時のコールバック
   */
  virtual void onButtonReleased(SDL_Event& ev) = 0;

  /**
   * ボタンがホバーされた時のコールバック
   */
  virtual void onMouseHovered() = 0;

  /**
   * ボタンがアンホバーされた時のコールバック
   */
  virtual void onMouseUnhovered() = 0;
};

enum class ButtonState {
  Normal,
  Hovered,
  Pressed,
};

/**
 * 基礎的なボタンの機能を提供するクラス
 */
class ImageButton : public Renderable, public ButtonEventCallback {
 public:
  ImageButton(Renderer& r, std::string name, int x, int y,
              std::string path_normal, std::string path_hovered = "",
              std::string path_pressed = "");

  const std::string& buttonName() { return name_; }

  // FixedRenderable
  void render() override final;

  // ButtonEventCallback
  void onButtonPressed(SDL_Event& ev) override;
  void onButtonReleased(SDL_Event& ev) override;
  void onMouseHovered() override;
  void onMouseUnhovered() override;

 protected:
  StatefulObjectManager<ImageTexture, ButtonState> state_manager_;

  bool isMouseHovered();
  bool isMouseUnhovered();
  bool isMouseLeftButtonPressed(SDL_Event& ev);
  bool isMouseLeftButtonReleased(SDL_Event& ev);

 private:
  std::string name_;

  const int x_;
  const int y_;
};

ImageButton::ImageButton(Renderer& r, std::string name, int x, int y,
                         std::string path_normal, std::string path_hovered,
                         std::string path_pressed)
    : Renderable(r), x_(x), y_(y) {
  auto texture_factory = ImageTextureFactory(r);
  state_manager_.setInitStatefulObject(
      ButtonState::Normal,
      texture_factory.create(path_normal, name + "_normal"));
  // Bind object
  if (!path_hovered.empty()) {
    state_manager_.bindStatefulObject(
        ButtonState::Hovered,
        texture_factory.create(path_hovered, name + "_hovered"));
  }
  if (!path_pressed.empty()) {
    state_manager_.bindStatefulObject(
        ButtonState::Pressed,
        texture_factory.create(path_pressed, name + "_pressed"));
  }
  // define state transition
  state_manager_.setStateTransition(ButtonState::Hovered, ButtonState::Pressed);
  state_manager_.setStateTransition(ButtonState::Pressed, ButtonState::Hovered);
  state_manager_.setStateTransition(ButtonState::Normal, ButtonState::Hovered);
  state_manager_.setStateTransition(ButtonState::Hovered, ButtonState::Normal);
}

void ImageButton::render() {
  auto& active_state = state_manager_.activeStateObject();
  SDL_Rect render_rect = {x_, y_, active_state.width(), active_state.height()};
  SDL_RenderCopy(renderer_.entity(), active_state.entity(),
                 nullptr /* TODO: introduce clip settings */, &render_rect);
}

void ImageButton::onButtonPressed(SDL_Event& ev) {
  auto& current_texture = state_manager_.activeStateObject();
  if (isMouseLeftButtonPressed(ev) &&
      state_manager_.activeState() == ButtonState::Hovered) {
    log(LogLevel::INFO, "State changed from Hovered to Pressed");
    state_manager_.stateTransition(ButtonState::Pressed);
  }
}

void ImageButton::onButtonReleased(SDL_Event& ev) {
  auto& current_texture = state_manager_.activeStateObject();
  if (isMouseLeftButtonReleased(ev) &&
      state_manager_.activeState() == ButtonState::Pressed) {
    log(LogLevel::INFO, "State changed from Pressed to Hovered");
    state_manager_.stateTransition(ButtonState::Hovered);
  }
}

void ImageButton::onMouseHovered() {
  if (isMouseHovered() && state_manager_.activeState() == ButtonState::Normal) {
    log(LogLevel::INFO, "State changed from Normal to Hovered");
    state_manager_.stateTransition(ButtonState::Hovered);
  }
}

void ImageButton::onMouseUnhovered() {
  if (isMouseUnhovered() &&
      state_manager_.activeState() == ButtonState::Hovered) {
    log(LogLevel::INFO, "State changed from Hovered to Normal");
    state_manager_.stateTransition(ButtonState::Normal);
  }
}

bool ImageButton::isMouseHovered() {
  int mouse_x, mouse_y;
  SDL_GetMouseState(&mouse_x, &mouse_y);
  return x_ < mouse_x &&
         mouse_x < x_ + state_manager_.activeStateObject().width() &&
         y_ < mouse_y &&
         mouse_y < y_ + state_manager_.activeStateObject().height();
}

bool ImageButton::isMouseUnhovered() {
  int mouse_x, mouse_y;
  SDL_GetMouseState(&mouse_x, &mouse_y);
  return !(x_ < mouse_x &&
           mouse_x < x_ + state_manager_.activeStateObject().width() &&
           y_ < mouse_y &&
           mouse_y < y_ + state_manager_.activeStateObject().height());
}

bool ImageButton::isMouseLeftButtonPressed(SDL_Event& ev) {
  if (ev.type == SDL_MOUSEBUTTONDOWN && ev.button.button == SDL_BUTTON_LEFT) {
    return isMouseHovered();
  }
  return false;
}

bool ImageButton::isMouseLeftButtonReleased(SDL_Event& ev) {
  if (ev.type == SDL_MOUSEBUTTONUP && ev.button.button == SDL_BUTTON_LEFT) {
    return isMouseHovered();
  }
  return false;
}

}  // namespace Truffle

#endif  // Truffle_BUTTON_H
