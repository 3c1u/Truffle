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

#endif  // TRUFFLE_LOGGER_H
