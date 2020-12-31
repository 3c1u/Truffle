//
// Created by shikugawa on 2020/12/29.
//

#ifndef TRUFFLE_EVENT_PUBLISHER_H
#define TRUFFLE_EVENT_PUBLISHER_H

#include <SDL2/SDL.h>

#include "event.h"

namespace Truffle {

class TruffleEventPublisher {
 public:
  ~TruffleEventPublisher() = default;

  /**
   * イベント発行用スレッドのループを停止する
   */
  virtual void terminate() = 0;

  /**
   * SDL2で未定義のイベントを発行するためのループを開始する
   */
  virtual void run() = 0;
};

class MouseEventConsumer {
 public:
  virtual ~MouseEventConsumer() = default;

  /**
   * マウスポインタが指定領域の上にあるか否か
   *
   * @return bool
   */
  virtual bool isMousePointerHovered(int mouse_x, int mouse_y) = 0;

  virtual bool isMousePointerUnhovered(int mouse_x, int mouse_y) = 0;
};

class MouseEventPublisher : public TruffleEventPublisher {
 public:
  MouseEventPublisher(MouseEventConsumer& consumer);

  void terminate() override final { terminated_ = true; }
  void run() override final;

 private:
  MouseEventConsumer& consumer_;
  bool terminated_ = false;
};

MouseEventPublisher::MouseEventPublisher(MouseEventConsumer& consumer)
    : consumer_(consumer) {}

void MouseEventPublisher::run() {
  while (!terminated_) {
    int mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);

    if (consumer_.isMousePointerHovered(mouse_x, mouse_y)) {
      SDL_Event mouse_hovered_event;
      mouse_hovered_event.type = SDL_USEREVENT;
      mouse_hovered_event.user.code = EV_MOUSE_HOVERED;
      SDL_PushEvent(&mouse_hovered_event);
    } else if (consumer_.isMousePointerUnhovered(mouse_x, mouse_y)) {
      SDL_Event mouse_unhovered_event;
      mouse_unhovered_event.type = SDL_USEREVENT;
      mouse_unhovered_event.user.code = EV_MOUSE_UNHOVERED;
      SDL_PushEvent(&mouse_unhovered_event);
    }
  }
}

}  // namespace Truffle

#endif  // TRUFFLE_EVENT_PUBLISHER_H
