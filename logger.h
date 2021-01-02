//
// Created by shikugawa on 2020/12/19.
//

#ifndef TRUFFLE_LOGGER_H
#define TRUFFLE_LOGGER_H

#include "spdlog/spdlog.h"

namespace Truffle {

// Thread safe logger
static const std::shared_ptr<spdlog::logger> logger = spdlog::default_logger();

enum class LogLevel {
  DEBUG,
  INFO,
  WARN,
  ERROR,
};

void log(LogLevel lv, std::string&& message) {
  switch (lv) {
    case LogLevel::DEBUG:
      logger->debug(message);
      break;
    case LogLevel::INFO:
      logger->info(message);
      break;
    case LogLevel::WARN:
      logger->warn(message);
      break;
    case LogLevel::ERROR:
      logger->error(message);
      break;
  }
}

}  // namespace Truffle
#endif  // Truffle_LOGGER_H
