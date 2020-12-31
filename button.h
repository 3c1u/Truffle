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

enum class ButtonState {
  Normal,
  Hovered,
  Pressed,
};

/**
 * 任意のボタンを表現するクラスの基底となるクラス。
 * ボタンに必要なメンバ関数やコールバックを提供する。
 */
class ButtonBase : public Renderable {
 public:
  ButtonBase(const Renderer& r) : Renderable(r) {}

  virtual const std::string& name() const& = 0;

  // Renderable
  virtual void render() override {}

  /**
   * ボタンが押された時のコールバック
   */
  virtual void onButtonPressed() = 0;

  /**
   * ボタンが離された時のコールバック
   */
  virtual void onButtonReleased() = 0;

  /**
   * ボタンがホバーされた時のコールバック
   */
  virtual void onMouseHovered() = 0;

  /**
   * ボタンがアンホバーされた時のコールバック
   */
  virtual void onMouseUnhovered() = 0;

  virtual void _onButtonPressed(SDL_Event& ev) = 0;
  virtual void _onButtonReleased(SDL_Event& ev) = 0;
  virtual void _onMouseHovered() = 0;
  virtual void _onMouseUnhovered() = 0;
};

/**
 * 基礎的なボタンの機能を提供するクラス
 */
class ImageButton : public ButtonBase {
 public:
  /**
   * 画像テクスチャボタンのコンストラクタ
   *
   * @param r レンダラ
   * @param name 名前
   * @param x x座標
   * @param y y座標
   * @param path_normal 初期状態のテクスチャのパス
   * @param path_hovered ホバー状態のテクスチャのパス
   * @param path_pressed 押下時のテクスチャのパス
   */
  ImageButton(const Renderer& r, std::string name, int x, int y,
              std::string path_normal, std::string path_hovered = "",
              std::string path_pressed = "");

  // ButtonBase
  const std::string& name() const& override { return name_; }

  // Renderable
  void render() override final;

  // ButtonEventCallback
  virtual void onButtonPressed() override;
  virtual void onButtonReleased() override;
  virtual void onMouseHovered() override;
  virtual void onMouseUnhovered() override;

  void _onButtonPressed(SDL_Event&) override final;
  void _onButtonReleased(SDL_Event&) override final;
  void _onMouseHovered() override final;
  void _onMouseUnhovered() override final;

 protected:
  StatefulObjectManager<ImageTexture, ButtonState> state_manager;

  bool isMouseHovered();
  bool isMouseUnhovered();
  bool isMouseLeftButtonPressed(SDL_Event& ev);
  bool isMouseLeftButtonReleased(SDL_Event& ev);

 private:
  const std::string name_;

  const int x_;
  const int y_;
};

ImageButton::ImageButton(const Renderer& r, std::string name, int x, int y,
                         std::string path_normal, std::string path_hovered,
                         std::string path_pressed)
    : ButtonBase(r), x_(x), y_(y) {
  state_manager.setInitStatefulObject(ButtonState::Normal, renderer_,
                                      path_normal, name + "_normal", x_, y_);
  // Bind object
  if (!path_hovered.empty()) {
    state_manager.bindStatefulObject(ButtonState::Hovered, renderer_,
                                     path_hovered, name + "_hovered", x_, y_);
  }
  if (!path_pressed.empty()) {
    state_manager.bindStatefulObject(ButtonState::Pressed, renderer_,
                                     path_pressed, name + "_pressed", x_, y_);
  }
  // define state transition
  state_manager.setStateTransition(ButtonState::Hovered, ButtonState::Pressed);
  state_manager.setStateTransition(ButtonState::Pressed, ButtonState::Hovered);
  state_manager.setStateTransition(ButtonState::Normal, ButtonState::Hovered);
  state_manager.setStateTransition(ButtonState::Hovered, ButtonState::Normal);
}

void ImageButton::render() {
  auto& active_state = state_manager.activeStateObject();
  SDL_Rect render_rect = {x_, y_, active_state.width(), active_state.height()};
  SDL_RenderCopy(const_cast<SDL_Renderer*>(renderer_.entity()),
                 const_cast<SDL_Texture*>(active_state.entity()),
                 nullptr /* TODO: introduce clip settings */, &render_rect);
}

void ImageButton::onButtonPressed() {
  log(LogLevel::INFO, "State changed from Hovered to Pressed");
  state_manager.stateTransition(ButtonState::Pressed);
}

void ImageButton::onButtonReleased() {
  log(LogLevel::INFO, "State changed from Pressed to Hovered");
  state_manager.stateTransition(ButtonState::Hovered);
}

void ImageButton::onMouseHovered() {
  log(LogLevel::INFO, "State changed from Normal to Hovered");
  state_manager.stateTransition(ButtonState::Hovered);
}

void ImageButton::onMouseUnhovered() {
  log(LogLevel::INFO, "State changed from Hovered to Normal");
  state_manager.stateTransition(ButtonState::Normal);
}

void ImageButton::_onButtonPressed(SDL_Event& ev) {
  auto& current_texture = state_manager.activeStateObject();
  if (isMouseLeftButtonPressed(ev) &&
      state_manager.activeState() == ButtonState::Hovered) {
    onButtonPressed();
  }
}

void ImageButton::_onButtonReleased(SDL_Event& ev) {
  auto& current_texture = state_manager.activeStateObject();
  if (isMouseLeftButtonReleased(ev) &&
      state_manager.activeState() == ButtonState::Pressed) {
    onButtonReleased();
  }
}

void ImageButton::_onMouseHovered() {
  if (isMouseHovered() && state_manager.activeState() == ButtonState::Normal) {
    onMouseHovered();
  }
}

void ImageButton::_onMouseUnhovered() {
  if (isMouseUnhovered() &&
      state_manager.activeState() == ButtonState::Hovered) {
    onMouseUnhovered();
  }
}

bool ImageButton::isMouseHovered() {
  int mouse_x, mouse_y;
  SDL_GetMouseState(&mouse_x, &mouse_y);
  return x_ < mouse_x &&
         mouse_x < x_ + state_manager.activeStateObject().width() &&
         y_ < mouse_y &&
         mouse_y < y_ + state_manager.activeStateObject().height();
}

bool ImageButton::isMouseUnhovered() {
  int mouse_x, mouse_y;
  SDL_GetMouseState(&mouse_x, &mouse_y);
  return !(x_ < mouse_x &&
           mouse_x < x_ + state_manager.activeStateObject().width() &&
           y_ < mouse_y &&
           mouse_y < y_ + state_manager.activeStateObject().height());
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
