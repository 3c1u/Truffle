/**
 * @file      text.h
 * @author    Rei Shimizu (shikugawa) <shikugawa@gmail.com>
 * @brief     Truffle text object
 *
 * @copyright Copyright 2021 Rei Shimizu. All rights reserved.
 */

#ifndef TRUFFLE_TEXT_H
#define TRUFFLE_TEXT_H

#include "engine/object.h"
#include "wrapper/sdl2/renderer_storage.h"
#include "wrapper/sdl2/texture.h"

namespace Truffle {

class SolidText : public TruffleVisibleObject {
 public:
  SolidText(std::string name, std::string text, int x, int y, Color color,
            std::string font, size_t font_size);

  void setText(std::string text);
  void render() final;

 private:
  Texture texture_;
  Color default_color_;
  std::string default_font_;
  size_t default_font_size_;
};

SolidText::SolidText(std::string name, std::string text, int x, int y,
                     Color color, std::string font, size_t font_size)
    : TruffleVisibleObject(name),
      texture_(text, TextTextureMode::Solid, FontInfo{font_size, font}, color),
      default_font_(font),
      default_font_size_(font_size),
      default_color_(color) {
  setPoint(x, y);
  setWidth(texture_.width());
  setHeight(texture_.height());
}

void SolidText::setText(std::string text) {
  auto new_texture =
      Texture(text, TextTextureMode::Solid,
              FontInfo{default_font_size_, default_font_}, default_color_);
  texture_ = new_texture;
}

void SolidText::render() {
  if (do_render_) {
    SDL_RenderCopy(const_cast<SDL_Renderer*>(
                       RendererStorage::get().activeRenderer()->entity()),
                   const_cast<SDL_Texture*>(texture_.entity()), nullptr,
                   &renderRect());
  }
}

}  // namespace Truffle

#endif  // TRUFFLE_TEXT_H
