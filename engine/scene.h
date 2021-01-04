/**
 * @file      scene.h
 * @author    Rei Shimizu (shikugawa) <shikugawa@gmail.com>
 * @brief     The definition of scene
 *
 * @copyright Copyright 2021 Rei Shimizu. All rights reserved.
 */

#ifndef TRUFFLE_SCENE_H
#define TRUFFLE_SCENE_H

#include <absl/container/flat_hash_map.h>

#include <string>

#include "common/non_copyable.h"
#include "controller.h"

namespace Truffle {

class TruffleScene : NonCopyable {
 public:
  explicit TruffleScene(std::string scene_name);

  /**
   * シーンの初期化を行う。
   */
  void initScene() const&;

  /**
   * コントローラーを追加する。
   * @param controller
   */
  void setController(TruffleController& controller);

  /**
   * シーンが管理するコントローラーの参照を返す。
   * @return
   */
  [[nodiscard]] const absl::flat_hash_map<std::string, TruffleControllerRef>&
  controllers() const& {
    return controllers_;
  }

  [[nodiscard]] const std::string& name() const& { return name_; }

 private:
  std::string name_;
  absl::flat_hash_map<std::string, TruffleControllerRef> controllers_;
};

}  // namespace Truffle

#endif  // TRUFFLE_SCENE_H
