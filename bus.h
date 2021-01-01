//
// Created by shikugawa on 2020/12/29.
//

#ifndef TRUFFLE_BUS_H
#define TRUFFLE_BUS_H

#include <absl/container/flat_hash_map.h>
#include <absl/strings/str_format.h>

#include <queue>

#include "exception.h"
#include "logger.h"
#include "message.h"
#include "non_copyable.h"
#include "singleton.h"

namespace Truffle {

class EventMessageBus : public MutableSingleton<EventMessageBus>, NonCopyable {
 public:
  static constexpr uint32_t PENDING_MESSAGE_SIZE_LIMIT = 1024;

  /**
   * 各ビヘイビアに属するメッセージキューのポインタを返す。もしキューが存在しなければ作成して返す。
   * @param behavior_name
   * @return
   */
  std::shared_ptr<std::queue<Message>> getMessageQueue(
      std::string behavior_name);

  /**
   * 指定したビヘイビアにメッセージを送る。宛先が存在しない場合は例外を送出し、受信バッファのメッセージ数制限に達した場合はfalseを返す。
   * @tparam T
   * @param dst_behavior
   * @param msg
   * @return
   */
  template <class T>
  bool sendMessage(std::string dst_behavior, T&& msg) {
    if (message_queue_.find(dst_behavior) == message_queue_.end()) {
      throw TruffleException(
          absl::StrFormat("%s behavior not found", dst_behavior));
    }
    if (message_queue_[dst_behavior]->size() >= PENDING_MESSAGE_SIZE_LIMIT) {
      log(LogLevel::WARN,
          absl::StrFormat(
              "Pending message size to %s had exceeded buffer size limit %i",
              dst_behavior, PENDING_MESSAGE_SIZE_LIMIT));
      return false;
    }
    // メッセージキューが到着順に構成されることを保証する
    std::unique_lock<std::mutex> lock(mux_);
    message_queue_[dst_behavior]->emplace(msg);
    return true;
  }

 private:
  friend class MutableSingleton<EventMessageBus>;

  EventMessageBus() = default;

  absl::flat_hash_map<std::string, std::shared_ptr<std::queue<Message>>>
      message_queue_;
  std::mutex mux_;
};

std::shared_ptr<std::queue<Message>> EventMessageBus::getMessageQueue(
    std::string behavior_name) {
  if (message_queue_.find(behavior_name) == message_queue_.end()) {
    auto queue = std::make_shared<std::queue<Message>>();
    message_queue_.emplace(behavior_name, queue);
  }
  return message_queue_.at(behavior_name);
}

}  // namespace Truffle
#endif  // TRUFFLE_BUS_H
