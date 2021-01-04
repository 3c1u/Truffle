/**
 * @file      object.h
 * @author    Rei Shimizu (shikugawa) <shikugawa@gmail.com>
 * @brief     The definition of object
 *
 * @copyright Copyright 2021 Rei Shimizu. All rights reserved.
 */

#ifndef TRUFFLE_OBJECT_H
#define TRUFFLE_OBJECT_H

#include <SDL2/SDL.h>

#include <forward_list>
#include <string>

#include "common/non_copyable.h"
#include "event.h"

namespace Truffle {

class TruffleVisibleObject : public NonCopyable {
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

  /**
   * メンバに対する描画処理を定義する
   */
  virtual void render() = 0;

  /**
   * 描画を有効にする
   */
  void enableRender() { do_render_ = true; }

  /**
   * 描画を無効にする
   */
  void disableRender() { do_render_ = false; }

  const std::string& name() const& { return name_; }
  const SDL_Rect& renderRect() const& { return render_rect; }

 protected:
  TruffleVisibleObject(std::string name);

  /**
   * イベントハンドラーを登録する。
   * @param callback
   */
  void setEventCallback(CustomEventCallback callback) {
    callback_.push_front(callback);
  }

  bool do_render_ = true;

 private:
  std::string name_;
  SDL_Rect render_rect;
  std::forward_list<CustomEventCallback> callback_;
};

using TruffleVisibleObjectRef = std::reference_wrapper<TruffleVisibleObject>;

class TruffleInvisibleObject : NonCopyable {
 public:
  /**
   * オブジェクトに属するイベントハンドラーを取得する
   * @return
   */
  std::forward_list<CustomEventCallback> eventCallbacks() { return callback_; }

  const std::string& name() const& { return name_; }

 protected:
  TruffleInvisibleObject(std::string name);

  /**
   * イベントハンドラーを登録する。
   * @param callback
   */
  void setEventCallback(CustomEventCallback callback) {
    callback_.push_front(callback);
  }

 private:
  std::string name_;
  std::forward_list<CustomEventCallback> callback_;
};

using TruffleInvisibleObjectRef =
    std::reference_wrapper<TruffleInvisibleObject>;

}  // namespace Truffle

#endif  // TRUFFLE_OBJECT_H
