/**
 * @file      bus.h
 * @author    Rei Shimizu (shikugawa) <shikugawa@gmail.com>
 * @brief     Event communication bus
 *
 * @copyright Copyright 2021 Rei Shimizu. All rights reserved.
 */


#ifndef TRUFFLE_BUS_H
#define TRUFFLE_BUS_H

#include <absl/container/flat_hash_map.h>
#include <absl/strings/str_format.h>

#include <memory>
#include <queue>

#include "common/exception.h"
#include "common/logger.h"
#include "common/non_copyable.h"
#include "common/singleton.h"

namespace Truffle {

struct Message {
  // 宛先オブジェクト
  std::string dst_object;
  std::string detail;
};

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


}  // namespace Truffle
#endif  // TRUFFLE_BUS_H
