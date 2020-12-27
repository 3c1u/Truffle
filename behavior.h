//
// Created by shikugawa on 2020/12/26.
//

#ifndef TRUFFLE_BEHAVIOR_H
#define TRUFFLE_BEHAVIOR_H

#include "event_callback.h"

namespace Truffle {

class TruffleBehavior : public InterruptionEventCallback {
 public:
  TruffleBehavior(std::string name) : name_(name) {}

  virtual ~TruffleBehavior() = default;

  virtual void start(){};

  virtual void update(){};

  const std::string& name() { return name_; }

 private:
  std::string name_;
};

}  // namespace Truffle

#endif  // Truffle_BEHAVIOR_H
