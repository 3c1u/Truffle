//
// Created by shikugawa on 2020/12/19.
//

#ifndef TRUFFLE_RENDERABLE_H
#define TRUFFLE_RENDERABLE_H

#include "renderer.h"

namespace Truffle {

class Renderable {
 public:
  Renderable(const Renderer& r) : renderer_(r) {}

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

 protected:
  const Renderer& renderer_;
  bool do_render_ = true;
};

}  // namespace Truffle

#endif  // Truffle_RENDERABLE_H
