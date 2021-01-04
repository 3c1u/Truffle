/**
 * @file      text.cpp
 * @author    Rei Shimizu (shikugawa) <shikugawa@gmail.com>
 * @brief     Truffle text object
 *
 * @copyright Copyright 2021 Rei Shimizu. All rights reserved.
 */

#include "text.h"

#include "wrapper/sdl2/renderer_storage.h"

namespace Truffle {

Text::Text(std::string name, std::string text, int x, int y,
           TextTextureMode mode, Color color, std::string font,
           size_t font_size)
    : TruffleVisibleObject(name),
      texture_(text, mode, FontInfo{font_size, font}, color),
      default_mode_(mode),
      default_font_(font),
      default_font_size_(font_size),
      default_color_(color) {
  setPoint(x, y);
  setWidth(texture_.width());
  setHeight(texture_.height());
}

void Text::setText(std::string text) {
  auto new_texture =
      Texture(text, default_mode_, FontInfo{default_font_size_, default_font_},
              default_color_);
  texture_ = new_texture;
}

void Text::render() {
  if (do_render_) {
    SDL_RenderCopy(const_cast<SDL_Renderer*>(
                       RendererStorage::get().activeRenderer()->entity()),
                   const_cast<SDL_Texture*>(texture_.entity()), nullptr,
                   &renderRect());
  }
}

}  // namespace Truffle