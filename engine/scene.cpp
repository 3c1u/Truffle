/**
 * @file      scene.cpp
 * @author    Rei Shimizu (shikugawa) <shikugawa@gmail.com>
 * @brief
 *
 * @copyright Copyright 2021 Rei Shimizu. All rights reserved.
 */

#include "scene.h"

#include <absl/strings/str_format.h>

#include "common/exception.h"
#include "common/logger.h"

namespace Truffle {

TruffleScene::TruffleScene(std::string scene_name) : name_(scene_name) {}

void TruffleScene::initScene() const& {
  for (const auto& [_, cb] : controllers_) {
    cb.get().start();
  }
}

void TruffleScene::setController(TruffleController& controller) {
  if (controllers_.find(controller.name()) != controllers_.end()) {
    throw TruffleException(absl::StrFormat(
        "controller %s had already registered", controller.name()));
  }
  // log_(LogLevel::INFO, absl::StrFormat("controller %s registered to scene
  // %s",
  //                                      controller.name(), name_));
  controllers_.emplace(controller.name(), controller);
}

}  // namespace Truffle