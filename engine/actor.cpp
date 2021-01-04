/**
 * @file      actor.cpp
 * @author    Rei Shimizu (shikugawa) <shikugawa@gmail.com>
 * @brief     Actor interface for actor-model
 *
 * @copyright Copyright 2021 Rei Shimizu. All rights reserved.
 */

#include "actor.h"

namespace Truffle {

Actor::Actor(Address address) : address_(address) {
  ActorTable::get().add(address_, *this);
}

std::optional<Message> Actor::recv() {
  if (message_queue_.empty()) return std::nullopt;
  auto& message = message_queue_.front();
  message_queue_.pop();
  return message;
}

std::optional<ActorRef> ActorTable::lookup_(Address address) {
  // O(N) lookup
  if (table_.find(address.controller) == table_.end()) {
    Logger::log(LogLevel::DEBUG,
                absl::StrFormat("Failed to lookup %s.%s", address.controller,
                                address.object));
    return std::nullopt;
  }
  for (auto&& actor : table_[address.controller]) {
    if (actor.get().address().object == address.object) {
      Logger::log(LogLevel::DEBUG,
                  absl::StrFormat("Succeeded to lookup %s.%s",
                                  address.controller, address.object));
      return actor;
    }
  }
  Logger::log(LogLevel::DEBUG,
              absl::StrFormat("Failed to lookup %s.%s", address.controller,
                              address.object));
  return std::nullopt;
}

void ActorTable::add_(Address address, Actor& actor) {
  // O(N) append
  if (table_.find(address.controller) == table_.end()) {
    table_.emplace(address.controller, std::vector<ActorRef>{actor});
    return;
  }
  for (const auto& actor : table_[address.controller]) {
    if (actor.get().address().object == address.object) {
      return;
    }
  }
  table_[address.controller].emplace_back(actor);
}

}  // namespace Truffle
