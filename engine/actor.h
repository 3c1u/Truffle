/**
 * @file      actor.h
 * @author    Rei Shimizu (shikugawa) <shikugawa@gmail.com>
 * @brief     Actor interface for actor-model
 *
 * @copyright Copyright 2021 Rei Shimizu. All rights reserved.
 */

#ifndef TRUFFLE_ACTOR_H
#define TRUFFLE_ACTOR_H

#include <absl/container/flat_hash_map.h>
#include <absl/strings/str_format.h>

#include <optional>
#include <queue>
#include <string>

#include "common/logger.h"
#include "common/singleton.h"

namespace Truffle {

struct Message {
  std::string detail;
};

struct Address {
  std::string controller;
  std::string object;
};

class Actor {
 public:
  static constexpr uint32_t PENDING_MESSAGE_SIZE_LIMIT = 1024;

  Actor(Address address);

  /**
   * メッセージを発行する
   */
  template <class T>
  void send(T&& message);

  /**
   * メッセージを1件受け取る
   * @return
   */
  std::optional<Message> recv();

  [[nodiscard]] const Address& address() const& { return address_; }

 private:
  const Address address_;
  std::queue<Message> message_queue_;
};

template <class T>
void Actor::send(T&& message) {
  if (message_queue_.size() >= PENDING_MESSAGE_SIZE_LIMIT) {
    Logger::log(
        LogLevel::WARN,
        absl::StrFormat(
            "Pending message size to %s.%s had exceeded buffer size limit %i",
            address_.controller, address_.object, PENDING_MESSAGE_SIZE_LIMIT));
    return;
  }
  // メッセージが発行された順に到着することを保証しない
  message_queue_.emplace(message);
}

using ActorRef = std::reference_wrapper<Actor>;

class ActorTable : public MutableSingleton<ActorTable> {
 public:
  /**
   * テーブルを検索し、検索が成功すればアクターの参照を返す
   * @param address
   * @return
   */
  static std::optional<ActorRef> lookup(Address address) {
    return ActorTable::get().lookup_(address);
  }

  /**
   * テーブルにオブジェクトのアドレスとアクターの参照を追加する
   * @param address
   * @param actor
   */
  static void add(Address address, Actor& actor) {
    ActorTable::get().add_(address, actor);
  }

 private:
  friend class MutableSingleton<ActorTable>;

  ActorTable() = default;

  std::optional<ActorRef> lookup_(Address address);
  void add_(Address address, Actor& actor);

  absl::flat_hash_map<std::string, std::vector<ActorRef>> table_;
};

}  // namespace Truffle

#endif  // TRUFFLE_ACTOR_H
