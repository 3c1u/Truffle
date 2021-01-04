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
#include "wrapper/sdl2/texture.h"

namespace Truffle {

class Text : public TruffleVisibleObject {
 public:
  Text(std::string name, std::string text, int x, int y, TextTextureMode mode,
       Color color, std::string font, size_t font_size);

  void setText(std::string text);
  void render() final;

 private:
  Texture texture_;
  TextTextureMode default_mode_;
  Color default_color_;
  std::string default_font_;
  size_t default_font_size_;
  std::string current_text_;
};

}  // namespace Truffle

#endif  // TRUFFLE_TEXT_H
