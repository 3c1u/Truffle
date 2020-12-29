//
// Created by shikugawa on 2020/12/19.
//

#ifndef TRUFFLE_RENDERABLE_H
#define TRUFFLE_RENDERABLE_H

#include <memory>
#include <type_traits>

#include "renderer.h"

namespace Truffle {

class Renderable {
 public:
  Renderable(Renderer& r, std::string name) : renderer_(r), name_(name) {}

  const std::string& renderableName() { return name_; }

  /**
   * Renderableを継承したクラスが、メンバに対する描画処理を定義する
   */
  virtual void render() = 0;

 protected:
  Renderer& renderer_;

 private:
  std::string name_;
};

}  // namespace Truffle

#endif  // Truffle_RENDERABLE_H
