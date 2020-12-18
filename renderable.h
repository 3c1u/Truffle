//
// Created by shikugawa on 2020/12/19.
//

#ifndef MIRAI_RENDERABLE_H
#define MIRAI_RENDERABLE_H

#include <memory>

namespace Mirai {
class Renderable {
 public:
  ~Renderable() = default;

  virtual void render() = 0;
};

using RenderablePtr = std::shared_ptr<Renderable>;
}  // namespace Mirai

#endif  // Mirai_RENDERABLE_H
