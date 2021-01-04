/**
 * @file      actor.h
 * @author    Rei Shimizu (shikugawa) <shikugawa@gmail.com>
 * @brief     Actor interface for actor-model
 *
 * @copyright Copyright 2021 Rei Shimizu. All rights reserved.
 */

#ifndef TRUFFLE_ACTOR_H
#define TRUFFLE_ACTOR_H

#include <absl/strings/str_format.h>

#include <optional>
#include <queue>
#include <string>

#include "common/logger.h"

namespace Truffle {

struct Message {
  std::string detail;
};

class Actor {
 public:
  static constexpr uint32_t PENDING_MESSAGE_SIZE_LIMIT = 1024;

  Actor(std::string name) : name_(name) {
    // ActorTable::get().add(controller.object, *this);
  }

  /**
   * メッセージを発行する
   */
  template <class T>
  void send(T&& message) {
    if (message_queue_.size() >= PENDING_MESSAGE_SIZE_LIMIT) {
      Logger::log(
          LogLevel::WARN,
          absl::StrFormat(
              "Pending message size to %s had exceeded buffer size limit %i",
              name_, PENDING_MESSAGE_SIZE_LIMIT));
      return false;
    }
    // メッセージが発行された順に到着することを保証しない
    message_queue_.emplace(message);
  }

  /**
   * メッセージを1件受け取る
   * @return
   */
  std::optional<Message> recv() {
    if (message_queue_.empty()) return std::nullopt;
    auto& message = message_queue_.front();
    message_queue_.pop();
    return message;
  }

  [[nodiscard]] const std::string& name() const& { return name_; }

 private:
  const std::string name_;
  std::queue<Message> message_queue_;
};

using ActorRef = std::reference_wrapper<Actor>;

}  // namespace Truffle

#endif  // TRUFFLE_ACTOR_H
