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
#include "message.h"
#include "stateful_object_manager.h"
#include "texture.h"

namespace Truffle {

enum class ButtonState {
  Normal,
  Hovered,
  Pressed,
};

class ButtonCallback {
 public:
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

 protected:
  bool isMouseHovered(const SDL_Rect& render_rect);
  bool isMouseUnhovered(const SDL_Rect& render_rect);
  bool isPressed(SDL_Event& ev, const SDL_Rect& render_rect);
  bool isReleased(SDL_Event& ev, const SDL_Rect& render_rect);
};

bool ButtonCallback::isMouseHovered(const SDL_Rect& render_rect) {
  int mouse_x, mouse_y;
  SDL_GetMouseState(&mouse_x, &mouse_y);
  return render_rect.x < mouse_x && mouse_x < render_rect.x + render_rect.w &&
         render_rect.y < mouse_y && mouse_y < render_rect.y + render_rect.h;
}

bool ButtonCallback::isMouseUnhovered(const SDL_Rect& render_rect) {
  return !isMouseHovered(render_rect);
}

bool ButtonCallback::isPressed(SDL_Event& ev, const SDL_Rect& render_rect) {
  if (ev.type == SDL_MOUSEBUTTONDOWN && ev.button.button == SDL_BUTTON_LEFT) {
    return isMouseHovered(render_rect);
  }
  return false;
}

bool ButtonCallback::isReleased(SDL_Event& ev, const SDL_Rect& render_rect) {
  if (ev.type == SDL_MOUSEBUTTONUP && ev.button.button == SDL_BUTTON_LEFT) {
    return isMouseHovered(render_rect);
  }
  return false;
}

/**
 * 基礎的なボタンの機能を提供するクラス
 */
class ImageButton : public Object, public ButtonCallback {
 public:
  /**
   * 画像テクスチャボタンのコンストラクタ
   *
   * @param parent_behavior 親ビヘイビア
   * @param renderer レンダラ
   * @param name 名前
   * @param x x座標
   * @param y y座標
   * @param path_normal 初期状態のテクスチャのパス
   * @param path_hovered ホバー状態のテクスチャのパス
   * @param path_pressed 押下時のテクスチャのパス
   */
  ImageButton(TruffleBehavior& parent_behavior, const Renderer& renderer,
              std::string name, int x, int y, std::string path_normal,
              std::string path_hovered = "", std::string path_pressed = "");

  // Renderable
  void render() override final;

  // ButtonEventCallback
  virtual void onButtonPressed() override;
  virtual void onButtonReleased() override;
  virtual void onMouseHovered() override;
  virtual void onMouseUnhovered() override;

  void _onButtonPressed(SDL_Event&) final;
  void _onButtonReleased(SDL_Event&) final;
  void _onMouseHovered() final;
  void _onMouseUnhovered() final;

  /**
   * 同一シーン内のビヘイビアにメッセージを送る。メッセージの送信に失敗した場合は例外を送出する。
   * 失敗した場合はfalseを返す。
   * @param dst_behavior
   * @param object_name
   * @param msg
   */
  void sendMessage(std::string dst_behavior, std::string dst_object,
                   Message& msg);
  void sendMessage(std::string dst_behavior, std::string dst_object,
                   Message&& msg);

 protected:
  StatefulObjectManager<ImageTexture, ButtonState> state_manager;
};

ImageButton::ImageButton(TruffleBehavior& behavior, const Renderer& renderer,
                         std::string name, int x, int y,
                         std::string path_normal, std::string path_hovered,
                         std::string path_pressed)
    : Object(behavior, renderer, name) {
  state_manager.setInitStatefulObject(ButtonState::Normal, behavior, renderer_,
                                      path_normal, name + "_normal", x, y);
  // Bind object
  if (!path_hovered.empty()) {
    state_manager.bindStatefulObject(ButtonState::Hovered, behavior, renderer_,
                                     path_hovered, name + "_hovered", x, y);
  }
  if (!path_pressed.empty()) {
    state_manager.bindStatefulObject(ButtonState::Pressed, behavior, renderer_,
                                     path_pressed, name + "_pressed", x, y);
  }
  // define state transition
  state_manager.setStateTransition(ButtonState::Hovered, ButtonState::Pressed);
  state_manager.setStateTransition(ButtonState::Pressed, ButtonState::Hovered);
  state_manager.setStateTransition(ButtonState::Normal, ButtonState::Hovered);
  state_manager.setStateTransition(ButtonState::Hovered, ButtonState::Normal);

  setPoint(state_manager.activeStateObject().renderRect().x,
           state_manager.activeStateObject().renderRect().y);
  setWidth(state_manager.activeStateObject().renderRect().w);
  setHeight(state_manager.activeStateObject().renderRect().h);

  // Register event callbacks
  setEventCallback([this] { this->_onMouseHovered(); });
  setEventCallback([this] { this->_onMouseUnhovered(); });
  setEventCallback([this](SDL_Event& e) { this->_onButtonReleased(e); });
  setEventCallback([this](SDL_Event& e) { this->_onButtonPressed(e); });
}

void ImageButton::render() {
  SDL_RenderCopy(
      const_cast<SDL_Renderer*>(renderer_.entity()),
      const_cast<SDL_Texture*>(state_manager.activeStateObject().entity()),
      nullptr /* TODO: introduce clip settings */, &renderRect());
}

void ImageButton::sendMessage(std::string dst_behavior, std::string dst_object,
                              Message& msg) {
  msg.dst_object = dst_object;
  Object::sendMessage(dst_behavior, msg);
}

void ImageButton::sendMessage(std::string dst_behavior, std::string dst_object,
                              Message&& msg) {
  msg.dst_object = dst_object;
  Object::sendMessage(dst_behavior, msg);
}

void ImageButton::onButtonPressed() {
  log(LogLevel::INFO, "State changed from Hovered to Pressed");
  state_manager.stateTransition(ButtonState::Pressed);
  setWidth(state_manager.activeStateObject().renderRect().w);
  setHeight(state_manager.activeStateObject().renderRect().h);
}

void ImageButton::onButtonReleased() {
  log(LogLevel::INFO, "State changed from Pressed to Hovered");
  state_manager.stateTransition(ButtonState::Hovered);
  setWidth(state_manager.activeStateObject().renderRect().w);
  setHeight(state_manager.activeStateObject().renderRect().h);
}

void ImageButton::onMouseHovered() {
  log(LogLevel::INFO, "State changed from Normal to Hovered");
  state_manager.stateTransition(ButtonState::Hovered);
  setWidth(state_manager.activeStateObject().renderRect().w);
  setHeight(state_manager.activeStateObject().renderRect().h);
}

void ImageButton::onMouseUnhovered() {
  log(LogLevel::INFO, "State changed from Hovered to Normal");
  state_manager.stateTransition(ButtonState::Normal);
  setWidth(state_manager.activeStateObject().renderRect().w);
  setHeight(state_manager.activeStateObject().renderRect().h);
}

void ImageButton::_onButtonPressed(SDL_Event& ev) {
  if (isPressed(ev, state_manager.activeStateObject().renderRect()) &&
      state_manager.activeState() == ButtonState::Hovered) {
    onButtonPressed();
  }
}

void ImageButton::_onButtonReleased(SDL_Event& ev) {
  if (isReleased(ev, state_manager.activeStateObject().renderRect()) &&
      state_manager.activeState() == ButtonState::Pressed) {
    onButtonReleased();
  }
}

void ImageButton::_onMouseHovered() {
  if (isMouseHovered(state_manager.activeStateObject().renderRect()) &&
      state_manager.activeState() == ButtonState::Normal) {
    onMouseHovered();
  }
}

void ImageButton::_onMouseUnhovered() {
  if (isMouseUnhovered(state_manager.activeStateObject().renderRect()) &&
      state_manager.activeState() == ButtonState::Hovered) {
    onMouseUnhovered();
  }
}

}  // namespace Truffle

#endif  // Truffle_BUTTON_H
