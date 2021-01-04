/**
 * @file      controller.cpp
 * @author    Rei Shimizu (shikugawa) <shikugawa@gmail.com>
 * @brief
 *
 * @copyright Copyright 2021 Rei Shimizu. All rights reserved.
 */

#include "controller.h"

#include "common/exception.h"

namespace Truffle {

TruffleController::TruffleController(std::string name) : name_(name) {}

void TruffleController::appendObject(TruffleVisibleObject& object) {
  if (visible_objects_.find(object.name()) != visible_objects_.end() ||
      invisible_objects_.find(object.name()) != invisible_objects_.end()) {
    throw TruffleException("Duplicated name object can't be registered");
  }
  visible_objects_.emplace(object.name(), object);
}

void TruffleController::appendObject(TruffleInvisibleObject& object) {
  if (visible_objects_.find(object.name()) != visible_objects_.end() ||
      invisible_objects_.find(object.name()) != invisible_objects_.end()) {
    throw TruffleException("Duplicated name object can't be registered");
  }
  invisible_objects_.emplace(object.name(), object);
}

}  // namespace Truffle