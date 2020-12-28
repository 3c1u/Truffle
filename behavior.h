//
// Created by shikugawa on 2020/12/26.
//

#ifndef TRUFFLE_BEHAVIOR_H
#define TRUFFLE_BEHAVIOR_H

#include "renderable.h"

namespace Truffle {

class TruffleBehavior : public FixedRenderable {
 public:
  TruffleBehavior(Renderer& r, std::string name)
      : FixedRenderable(r, name), name_(name) {}

  virtual ~TruffleBehavior() = default;

  virtual void start(){};

  virtual void update(SDL_Event&){};

  const std::string& behaviorName() { return name_; }

 private:
  std::string name_;
};

}  // namespace Truffle

#endif  // Truffle_BEHAVIOR_H
