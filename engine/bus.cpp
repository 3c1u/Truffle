/**
 * @file      bus.cpp
 * @author    Rei Shimizu (shikugawa) <shikugawa@gmail.com>
 * @brief     Event communication bus
 *
 * @copyright Copyright 2021 Rei Shimizu. All rights reserved.
 */

#include "bus.h"

namespace Truffle {

std::shared_ptr<std::queue<Message>> EventMessageBus::getMessageQueue(
    std::string controller_name) {
  if (message_queue_.find(controller_name) == message_queue_.end()) {
    auto queue = std::make_shared<std::queue<Message>>();
    message_queue_.emplace(controller_name, queue);
  }
  return message_queue_.at(controller_name);
}

}  // namespace Truffle
