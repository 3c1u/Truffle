//
// Created by shikugawa on 2020/12/19.
//

#ifndef MIRAI_LOGGER_H
#define MIRAI_LOGGER_H

#include "spdlog/spdlog.h"

namespace Mirai {

// Thread safe logger
static const std::shared_ptr<spdlog::logger> logger =
    spdlog::stdout_color_mt("console");

enum class LogLevel {
  INFO,
  WARN,
  ERROR,
};

void log(LogLevel lv, std::string&& message) {
  switch (lv) {
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

}  // namespace Mirai
#endif  // Mirai_LOGGER_H
