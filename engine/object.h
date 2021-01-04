/**
 * @file      object.h
 * @author    Rei Shimizu (shikugawa) <shikugawa@gmail.com>
 * @brief     The definition of object
 *
 * @copyright Copyright 2021 Rei Shimizu. All rights reserved.
 */

#ifndef TRUFFLE_OBJECT_H
#define TRUFFLE_OBJECT_H

#include <SDL.h>

#include <forward_list>
#include <string>

#include "common/non_copyable.h"

namespace Truffle {

using CustomEventCallback = std::function<void(SDL_Event&)>;

class TruffleObject : public NonCopyable {
 public:
  /**
   * オブジェクトの位置座標を更新する
   * @param x
   * @param y
   */
  void setPoint(int x, int y);

  /**
   * オブジェクトの幅を設定する
   * @param width
   */
  void setWidth(int width) { render_rect.w = width; }

  /**
   * オブジェクトの高さを設定する
   * @param height
   */
  void setHeight(int height) { render_rect.h = height; }

  /**
   * オブジェクトに属するイベントハンドラーを取得する
   * @return
   */
  std::forward_list<CustomEventCallback> eventCallbacks() { return callback_; }

  const std::string& name() const& { return name_; }
  const SDL_Rect& renderRect() const& { return render_rect; }

 protected:
  TruffleObject(std::string name);

  /**
   * イベントハンドラーを登録する。
   * @param callback
   */
  void setEventCallback(std::function<void(SDL_Event&)> callback) {
    callback_.push_front(callback);
  }

 private:
  std::string name_;
  SDL_Rect render_rect;
  std::forward_list<CustomEventCallback> callback_;
};

using TruffleObjectRef = std::reference_wrapper<TruffleObject>;

}  // namespace Truffle

#endif  // TRUFFLE_OBJECT_H
