/**
 * @file      button.cpp
 * @author    Rei Shimizu (shikugawa) <shikugawa@gmail.com>
 * @brief     Truffle button object
 *
 * @copyright Copyright 2021 Rei Shimizu. All rights reserved.
 */

#include "button.h"

#include "wrapper/sdl2/renderer_storage.h"

namespace Truffle {

ButtonCallback::ButtonCallback(std::string name) : TruffleVisibleObject(name) {}

void ButtonCallback::_onButtonPressed(Event& ev) {
  if (isPressed(ev, state_manager.activeStateObject().renderRect()) &&
      state_manager.activeState() == ButtonState::Hovered) {
    Logger::log(LogLevel::INFO, "State changed from Hovered to Pressed");
    state_manager.stateTransition(ButtonState::Pressed);
    setWidth(state_manager.activeStateObject().renderRect().w);
    setHeight(state_manager.activeStateObject().renderRect().h);
    onButtonPressed();
  }
}

void ButtonCallback::_onButtonReleased(Event& ev) {
  if (isReleased(ev, state_manager.activeStateObject().renderRect()) &&
      state_manager.activeState() == ButtonState::Pressed) {
    Logger::log(LogLevel::INFO, "State changed from Pressed to Hovered");
    state_manager.stateTransition(ButtonState::Hovered);
    setWidth(state_manager.activeStateObject().renderRect().w);
    setHeight(state_manager.activeStateObject().renderRect().h);
    onButtonReleased();
  }
}

void ButtonCallback::_onMouseHovered(Event& ev) {
  if (isMouseHovered(state_manager.activeStateObject().renderRect()) &&
      state_manager.activeState() == ButtonState::Normal) {
    Logger::log(LogLevel::INFO, "State changed from Normal to Hovered");
    state_manager.stateTransition(ButtonState::Hovered);
    setWidth(state_manager.activeStateObject().renderRect().w);
    setHeight(state_manager.activeStateObject().renderRect().h);
    onMouseHovered();
  }
}

void ButtonCallback::_onMouseUnhovered(Event& ev) {
  if (isMouseUnhovered(state_manager.activeStateObject().renderRect()) &&
      state_manager.activeState() == ButtonState::Hovered) {
    Logger::log(LogLevel::INFO, "State changed from Hovered to Normal");
    state_manager.stateTransition(ButtonState::Normal);
    setWidth(state_manager.activeStateObject().renderRect().w);
    setHeight(state_manager.activeStateObject().renderRect().h);
    onMouseUnhovered();
  }
}

bool ButtonCallback::isMouseHovered(const SDL_Rect& render_rect) {
  int mouse_x, mouse_y;
  SDL_GetMouseState(&mouse_x, &mouse_y);
  return render_rect.x < mouse_x && mouse_x < render_rect.x + render_rect.w &&
         render_rect.y < mouse_y && mouse_y < render_rect.y + render_rect.h;
}

bool ButtonCallback::isMouseUnhovered(const SDL_Rect& render_rect) {
  return !isMouseHovered(render_rect);
}

bool ButtonCallback::isPressed(Event& ev, const SDL_Rect& render_rect) {
  if (ev.type == SDL_MOUSEBUTTONDOWN && ev.button.button == SDL_BUTTON_LEFT) {
    return isMouseHovered(render_rect);
  }
  return false;
}

bool ButtonCallback::isReleased(Event& ev, const SDL_Rect& render_rect) {
  if (ev.type == SDL_MOUSEBUTTONUP && ev.button.button == SDL_BUTTON_LEFT) {
    return isMouseHovered(render_rect);
  }
  return false;
}

Button::Button(std::string controller_name, std::string object_name, int x,
               int y, std::string path_normal, std::string path_hovered,
               std::string path_pressed)
    : Actor(Address{controller_name, object_name}),
      ButtonCallback(object_name) {
  state_manager.setInitStatefulObject(ButtonState::Normal, path_normal,
                                      object_name + "_normal", x, y);
  // Bind object
  if (!path_hovered.empty()) {
    state_manager.bindStatefulObject(ButtonState::Hovered, path_hovered,
                                     object_name + "_hovered", x, y);
  }
  if (!path_pressed.empty()) {
    state_manager.bindStatefulObject(ButtonState::Pressed, path_pressed,
                                     object_name + "_pressed", x, y);
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
  setEventCallback([this](Event& e) { this->_onMouseHovered(e); });
  setEventCallback([this](Event& e) { this->_onMouseUnhovered(e); });
  setEventCallback([this](Event& e) { this->_onButtonReleased(e); });
  setEventCallback([this](Event& e) { this->_onButtonPressed(e); });
}

void Button::render() {
  if (do_render_) {
    SDL_RenderCopy(const_cast<SDL_Renderer*>(
                       RendererStorage::get().activeRenderer()->entity()),
                   const_cast<SDL_Texture*>(
                       state_manager.activeStateObject().texture().entity()),
                   nullptr /* TODO: introduce clip settings */, &renderRect());
  }
}

}  // namespace Truffle
