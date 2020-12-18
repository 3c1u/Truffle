//
// Created by shikugawa on 2020/12/19.
//

#ifndef MIRAI_EXCEPTION_H
#define MIRAI_EXCEPTION_H

#include <exception>
#include <stdexcept>
#include <string>

namespace Mirai {
class MiraiException : public std::runtime_error {
 public:
  MiraiException(const std::string& message) : std::runtime_error(message) {}
};

}  // namespace Mirai
#endif  // Mirai_EXCEPTION_H
