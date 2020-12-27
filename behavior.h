//
// Created by shikugawa on 2020/12/26.
//

#ifndef MIRAI_BEHAVIOR_H
#define MIRAI_BEHAVIOR_H

#include "event_callback.h"

namespace Mirai {

class MiraiBehavior : public KeyPressCallback {
 public:
  virtual ~MiraiBehavior() = default;

  virtual void start(){};

  virtual void update(){};
};

}  // namespace Mirai

#endif  // MIRAI_BEHAVIOR_H
