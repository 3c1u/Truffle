//
// Created by shikugawa on 2020/12/19.
//

#ifndef MIRAI_TEXTURE_H
#define MIRAI_TEXTURE_H

#include <iostream>

#include "renderable.h"

namespace Mirai {

class Texture : public Renderable {
 public:
  // Renderable
  void render() override { std::cout << "rendered" << std::endl; }
};
}  // namespace Mirai

#endif  // Mirai_TEXTURE_H
