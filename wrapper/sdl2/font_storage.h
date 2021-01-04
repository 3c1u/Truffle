/**
 * @file      font_storage.h
 * @author    Hikaru Terazono (3c1u) <3c1u@vulpesgames.tokyo>
 * @brief     Font storage type. Wrapper of sdl_ttf
 *
 * @copyright Copyright 2021 Hikaru Terazono. All rights reserved.
 */

#pragma once

#include <SDL_ttf.h>
#include <absl/container/flat_hash_map.h>

#include <memory>
#include <string>

#include "common/exception.h"
#include "common/logger.h"
#include "common/non_copyable.h"
#include "common/singleton.h"
#include "wrapper/sdl2/font.h"

namespace Truffle {

class FontStorage final : public ConstSingleton<FontStorage>,
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
  friend class ConstSingleton<FontStorage>;

  FontStorage();

  std::shared_ptr<Font> openFont_(std::string name, size_t size) const;
  void loadFont_(std::string name, std::string const& path) const;

  mutable absl::flat_hash_map<std::string, std::string> loaded_font_;
  mutable absl::flat_hash_map<std::string, std::vector<FontDriver>>
      active_font_;
};

}  // namespace Truffle
