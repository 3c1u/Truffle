//
// Created by shikugawa on 2020/12/19.
//

#ifndef TRUFFLE_RENDERABLE_H
#define TRUFFLE_RENDERABLE_H

#include "renderer.h"

namespace Truffle {

class Renderable {
 public:
  Renderable(Renderer& r) : renderer_(r) {}

  /**
   * Renderableを継承したクラスが、メンバに対する描画処理を定義する
   */
  virtual void render() = 0;

 protected:
  Renderer& renderer_;
};

}  // namespace Truffle

#endif  // Truffle_RENDERABLE_H
