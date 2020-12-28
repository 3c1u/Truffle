//
// Created by shikugawa on 2020/12/19.
//

#ifndef TRUFFLE_RENDERABLE_H
#define TRUFFLE_RENDERABLE_H

#include <memory>
#include <type_traits>

#include "renderer.h"

namespace Truffle {

class MovableRenderable;
class FixedRenderable;

// TODO: Reject to compile if user-defined class had derived Renderable class
class Renderable {
 public:
  Renderable(Renderer& renderer, std::string name)
      : renderer_(renderer), name_(name) {}

  const std::string& renderableName() { return name_; }

 protected:
  Renderer& renderer_;

 private:
  std::string name_;
};

class MovableRenderable : public Renderable {
 public:
  MovableRenderable(Renderer& r, std::string name) : Renderable(r, name) {}

  virtual void render(int x, int y) = 0;
};

class FixedRenderable : public Renderable {
 public:
  FixedRenderable(Renderer& r, std::string name) : Renderable(r, name) {}

  virtual void render() = 0;
};

}  // namespace Truffle

#endif  // Truffle_RENDERABLE_H
