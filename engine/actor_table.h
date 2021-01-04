/**
 * @file      actor_table.h
 * @author    Rei Shimizu (shikugawa) <shikugawa@gmail.com>
 * @brief     Actor table used by router to publish message
 *
 * @copyright Copyright 2021 Rei Shimizu. All rights reserved.
 */

#ifndef TRUFFLE_ACTOR_TABLE_H
#define TRUFFLE_ACTOR_TABLE_H

#include <absl/container/flat_hash_map.h>

#include <optional>
#include <string>
#include <vector>

#include "actor.h"
#include "common/singleton.h"

namespace Truffle {

struct Address {
  std::string controller;
  std::string object;
};

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
    ActorTable::get().add_(address);
  }

 private:
  friend class MutableSingleton<ActorTable>;

  ActorTable() = default;

  std::optional<ActorRef> lookup_(Address address);
  void add_(Address address, Actor& actor);

  absl::flat_hash_map<std::string, std::vector<ActorRef>> table_;
};

}  // namespace Truffle

#endif  // TRUFFLE_ACTOR_TABLE_H
