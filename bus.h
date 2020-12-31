//
// Created by shikugawa on 2020/12/29.
//

#ifndef TRUFFLE_BUS_H
#define TRUFFLE_BUS_H

#include <absl/container/flat_hash_map.h>

#include <queue>

namespace Truffle {

class Message {};

class EventMessageBus {
 public:
  /**
   * 各ビヘイビアに属するメッセージキューを
   * @param behavior_name
   * @return
   */
  std::queue<Message>& getMessageQueue(std::string behavior_name);

 private:
  absl::flat_hash_map<std::string, std::queue<Message>> message_queue_;
};

std::queue<Message>& EventMessageBus::getMessageQueue(
    std::string behavior_name) {
  assert(false);
}
}  // namespace Truffle
#endif  // TRUFFLE_BUS_H
