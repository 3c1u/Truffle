/**
 * @file      logger.h
 * @author    Rei Shimizu (shikugawa) <shikugawa@gmail.com>
 * @brief     spdlog wrapper
 *
 * @copyright Copyright 2021 Rei Shimizu. All rights reserved.
 */

#ifndef TRUFFLE_LOGGER_H
#define TRUFFLE_LOGGER_H

#include <spdlog/spdlog.h>

#include <memory>

namespace Truffle {

// Thread unsafe logger
static const std::shared_ptr<spdlog::logger> logger_ = spdlog::default_logger();

enum class LogLevel {
  DEBUG,
  INFO,
  WARN,
  ERROR,
};

class Logger {
 public:
  static void log(LogLevel lv, std::string&& message) {
    assert(logger_ != nullptr);
    switch (lv) {
      case LogLevel::DEBUG:
        logger_->debug(message);
        break;
      case LogLevel::INFO:
        logger_->info(message);
        break;
      case LogLevel::WARN:
        logger_->warn(message);
        break;
      case LogLevel::ERROR:
        logger_->error(message);
        break;
    }
  }
};

}  // namespace Truffle

#endif  // TRUFFLE_LOGGER_H
