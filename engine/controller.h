/**
 * @file      controller.h
 * @author    Rei Shimizu (shikugawa) <shikugawa@gmail.com>
 * @brief     The definition of controller
 *
 * @copyright Copyright 2021 Rei Shimizu. All rights reserved.
 */

#ifndef TRUFFLE_CONTROLLER_H
#define TRUFFLE_CONTROLLER_H

#include <SDL2/SDL.h>
#include <absl/container/flat_hash_map.h>

#include <string>

#include "wrapper/sdl2/event.h"
#include "common/non_copyable.h"
#include "object.h"

namespace Truffle {

class TruffleController : NonCopyable {
 public:
  /**
   * シーンの開始時に一度だけ実行されるコールバック
   */
  virtual void start(){};

  /**
   * 毎フレーム毎に1回呼ばれるコールバック
   */
  virtual void update(Event&){};

  /**
   * オブジェクトを追加する。名前が重複していれば例外を発生する。
   * @param object
   */
  void appendObject(TruffleVisibleObject& object);
  void appendObject(TruffleInvisibleObject& object);

  /**
   * このコントローラーによって管理されているすべての可視化対象オブジェクトを返す。
   * @return
   */
  [[nodiscard]] const absl::flat_hash_map<std::string, TruffleVisibleObjectRef>&
  visibleObjects() const& {
    return visible_objects_;
  }
  absl::flat_hash_map<std::string, TruffleVisibleObjectRef>&
  visibleObjects() & {
    return visible_objects_;
  }

  /**
   * このコントローラーによって管理されているすべての可視化対象外オブジェクトを返す。
   * @return
   */
  [[nodiscard]] const absl::flat_hash_map<std::string,
                                          TruffleInvisibleObjectRef>&
  invisibleObjects() const& {
    return invisible_objects_;
  }
  absl::flat_hash_map<std::string, TruffleInvisibleObjectRef>&
  invisibleObjects() & {
    return invisible_objects_;
  }

  [[nodiscard]] const std::string& name() const& { return name_; }

 protected:
  /**
   * コントローラーのコンストラクタ
   * @param name
   * コントローラーの名前。名前に重複があると例外が発生するので注意。
   */
  TruffleController(std::string name);

 private:
  absl::flat_hash_map<std::string, TruffleVisibleObjectRef> visible_objects_;
  absl::flat_hash_map<std::string, TruffleInvisibleObjectRef>
      invisible_objects_;
  std::string name_;
};

using TruffleControllerRef = std::reference_wrapper<TruffleController>;

}  // namespace Truffle

#endif  // TRUFFLE_CONTROLLER_H
