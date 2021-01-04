/**
 * @file      font_storage.cpp
 * @author    Rei Shimizu (shikugawa) <shikugawa@gmail.com>
 * @brief     Font storage type. Wrapper of sdl_ttf
 *
 * @copyright Copyright 2021 Rei Shimizu. All rights reserved.
 */

#include "font_storage.h"

namespace Truffle {

FontStorage::FontStorage() {
  if (TTF_WasInit() == 0) {
    // SDL2_ttf should be initialized once
    Logger::log(LogLevel::DEBUG, "SDL_ttf initialized");
    TTF_Init();
  }
}

std::shared_ptr<Font> FontStorage::openFont_(std::string name, size_t size) {
  if (loaded_font_.find(name) == loaded_font_.end()) {
    throw TruffleException(
        absl::StrFormat("Font %s must be loaded before open", name));
  }

  if (active_font_.find(name) == active_font_.end()) {
    active_font_.emplace(name, std::vector<FontDriver>());
    active_font_.at(name).emplace_back(
        FontDriver{size, std::make_shared<Font>(loaded_font_[name], size)});
    return active_font_.at(name).at(0).font_entity;
  }

  const auto& font_drivers = active_font_.at(name);

  for (const auto& font_driver : font_drivers) {
    if (font_driver.size == size) {
      return font_driver.font_entity;
    }
  }

  auto font_driver =
      FontDriver{size, std::make_shared<Font>(loaded_font_[name], size)};
  active_font_.at(name).emplace_back(font_driver);
  return font_driver.font_entity;
}

void FontStorage::loadFont_(std::string name, std::string const& path) {
  if (loaded_font_.find(name) != loaded_font_.end()) {
    return;
  }
  loaded_font_.emplace(name, path);
}

}  // namespace Truffle
