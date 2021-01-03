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
   * 各コントローラーに属するメッセージキューのポインタを返す。もしキューが存在しなければ作成して返す。
   * @param controller_name
   * @return
   */
  std::shared_ptr<std::queue<Message>> getMessageQueue(
      std::string controller_name);

  /**
   * 指定したコントローラーにメッセージを送る。宛先が存在しない場合は例外を送出し、受信バッファのメッセージ数制限に達した場合はfalseを返す。
   * @tparam T
   * @param dst_controller
   * @param msg
   * @return
   */
  template <class T>
  bool sendMessage(std::string dst_controller, T&& msg) {
    if (message_queue_.find(dst_controller) == message_queue_.end()) {
      throw TruffleException(
          absl::StrFormat("%s controller not found", dst_controller));
    }
    if (message_queue_[dst_controller]->size() >= PENDING_MESSAGE_SIZE_LIMIT) {
      log(LogLevel::WARN,
          absl::StrFormat(
              "Pending message size to %s had exceeded buffer size limit %i",
              dst_controller, PENDING_MESSAGE_SIZE_LIMIT));
      return false;
    }
    // メッセージキューが到着順に構成されることを保証する
    std::unique_lock<std::mutex> lock(mux_);
    message_queue_[dst_controller]->emplace(msg);
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
    std::string controller_name) {
  if (message_queue_.find(controller_name) == message_queue_.end()) {
    auto queue = std::make_shared<std::queue<Message>>();
    message_queue_.emplace(controller_name, queue);
  }
  return message_queue_.at(controller_name);
}

}  // namespace Truffle
#endif  // TRUFFLE_BUS_H
