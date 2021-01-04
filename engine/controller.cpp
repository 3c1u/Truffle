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

void TruffleController::appendObject(TruffleObject& object) {
  if (objects_.find(object.name()) != objects_.end()) {
    throw TruffleException("Duplicated name object can't be registered");
  }
  objects_.emplace(object.name(), object);
}

}  // namespace Truffle