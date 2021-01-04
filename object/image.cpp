/**
 * @file      image.cpp
 * @author    Rei Shimizu (shikugawa) <shikugawa@gmail.com>
 * @brief     Truffle image object
 *
 * @copyright Copyright 2021 Rei Shimizu. All rights reserved.
 */

#include "image.h"

#include "wrapper/sdl2/renderer_storage.h"

namespace Truffle {

Image::Image(std::string name, std::string path, int x, int y)
    : TruffleVisibleObject(name), texture_(path) {
  setPoint(x, y);
  setWidth(texture_.width());
  setHeight(texture_.height());
}

void Image::render() {
  if (do_render_) {
    SDL_RenderCopy(const_cast<SDL_Renderer*>(
                       RendererStorage::get().activeRenderer()->entity()),
                   const_cast<SDL_Texture*>(texture_.entity()), nullptr,
                   &renderRect());
  }
}

}  // namespace Truffle
