//
// Created by shikugawa on 2020/12/26.
//

#ifndef TRUFFLE_BEHAVIOR_H
#define TRUFFLE_BEHAVIOR_H

#include <forward_list>
#include <memory>

#include "renderable.h"
#include "stateful_object_manager.h"
#include "texture.h"

namespace Truffle {

class TruffleBehavior {
 public:
  TruffleBehavior(std::string name) : name_(name) {}

  virtual ~TruffleBehavior() = default;

  /**
   * シーンの開始時に一度だけ実行されるコールバック
   */
  virtual void start(){};

  /**
   * 毎フレーム毎に1回呼ばれるコールバック
   */
  virtual void update(SDL_Event&){};

  const std::string& behaviorName() { return name_; }

  const std::forward_list<std::reference_wrapper<Renderable>>&
  targetRenderableStates() {
    return renderables;
  }

 protected:
  std::forward_list<std::reference_wrapper<Renderable>> renderables;

 private:
  std::string name_;
};

}  // namespace Truffle

#endif  // Truffle_BEHAVIOR_H
