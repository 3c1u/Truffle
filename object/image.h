/**
 * @file      image.h
 * @author    Rei Shimizu (shikugawa) <shikugawa@gmail.com>
 * @brief     Truffle image object
 *
 * @copyright Copyright 2021 Rei Shimizu. All rights reserved.
 */

#ifndef TRUFFLE_IMAGE_H
#define TRUFFLE_IMAGE_H

#include "engine/object.h"
#include "wrapper/sdl2/texture.h"

namespace Truffle {

class Image : public TruffleVisibleObject {
 public:
  Image(std::string name, std::string path, int x, int y);

  void render() final;

 private:
  Texture texture_;
};

}  // namespace Truffle

#endif  // TRUFFLE_IMAGE_H
