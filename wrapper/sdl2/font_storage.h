/**
 * @file      font_storage.h
 * @author    Hikaru Terazono (3c1u) <3c1u@vulpesgames.tokyo>
 * @brief     Font storage type. Wrapper of sdl_ttf
 *
 * @copyright Copyright 2021 Hikaru Terazono. All rights reserved.
 */

#pragma once

#include <SDL2/SDL_ttf.h>
#include <absl/container/flat_hash_map.h>

#include <memory>
#include <string>

#include "common/exception.h"
#include "common/logger.h"
#include "common/non_copyable.h"
#include "common/singleton.h"
#include "wrapper/sdl2/font.h"

namespace Truffle {

class FontStorage final : public MutableSingleton<FontStorage>,
                          public NonCopyable {
 public:
  struct FontDriver {
    size_t size;
    std::shared_ptr<Font> font_entity;
  };

  /**
   * 初期化時にロードされたフォントを実際に生成する。ロードされていなかったら例外を返す。
   * @param name
   * @param size
   * @return
   */
  static std::shared_ptr<Font> openFont(std::string name, size_t size) {
    return FontStorage::get().openFont_(name, size);
  }

  /**
   * フォントをロードする。
   * @param name
   * @param path
   */
  static void loadFont(std::string name, std::string const& path) {
    return FontStorage::get().loadFont_(name, path);
  }

 private:
  friend class MutableSingleton<FontStorage>;

  explicit FontStorage() {
    if (TTF_WasInit() == 0) {
      // SDL2_ttf should be initialized once
      // log_(LogLevel::DEBUG, "SDL_ttf initialized");
      TTF_Init();
    }
  }

  std::shared_ptr<Font> openFont_(std::string name, size_t size) {
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
  void loadFont_(std::string name, std::string const& path) {
    if (loaded_font_.find(name) != loaded_font_.end()) {
      return;
    }
    loaded_font_.emplace(name, path);
  }

  absl::flat_hash_map<std::string, std::string> loaded_font_;
  absl::flat_hash_map<std::string, std::vector<FontDriver>> active_font_;
};

}  // namespace Truffle
