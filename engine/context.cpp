/**
 * @file      context.cpp
 * @author    Rei Shimizu (shikugawa) <shikugawa@gmail.com>
 * @brief     It holds global context, such as global controller / object
 *
 * @copyright Copyright 2021 Rei Shimizu. All rights reserved.
 */

#include "context.h"

namespace Truffle {

void IsolatedControllerStore::set_(TruffleController& controller) {
  if (controllers_.find(controller.name()) != controllers_.end()) {
    throw TruffleException(absl::StrFormat(
        "Failed to register controller %s to global controller store",
        controller.name()));
  }
  controllers_.emplace(controller.name(), controller);
}

void Context::setController_(TruffleController& controller) {
  IsolatedControllerStore::get().set(controller);
}

}  // namespace Truffle
