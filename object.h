//
// Created by 清水嶺 on 2021/01/02.
//

#ifndef TRUFFLE_OBJECT_H
#define TRUFFLE_OBJECT_H

#include <SDL2/SDL.h>

#include <forward_list>

#include "renderable.h"
#include "renderer.h"

namespace Truffle {

// TODO: implement
class ObjectGroup {};

class Object : public Renderable {
 public:
  const std::string& name() const& { return name_; }

  virtual void render() override {}

  const SDL_Rect& renderRect() const& { return render_rect; }

  void setPoint(int x, int y) {
    render_rect.x = x;
    render_rect.y = y;
  }
  void setWidth(int width) { render_rect.w = width; }
  void setHeight(int height) { render_rect.h = height; }

  std::forward_list<std::function<void(SDL_Event&)>>
  eventCallbacksWithDescriptor() {
    return callback_with_event_descriptor_;
  }
  std::forward_list<std::function<void()>> eventCallbacks() {
    return callback_;
  }

 protected:
  explicit Object(const Renderer& renderer, std::string name)
      : Renderable(renderer), name_(name) {}

  /**
   * TODO: この方法は嫌なので別のやりかたを考える
   * @param callback
   */
  void setEventCallback(std::function<void()> callback) {
    callback_.push_front(callback);
  }
  void setEventCallback(std::function<void(SDL_Event&)> callback) {
    callback_with_event_descriptor_.push_front(callback);
  }

 private:
  std::string name_;
  SDL_Rect render_rect;
  std::forward_list<std::function<void()>> callback_;
  std::forward_list<std::function<void(SDL_Event&)>>
      callback_with_event_descriptor_;
};

}  // namespace Truffle
#endif  // TRUFFLE_OBJECT_H
