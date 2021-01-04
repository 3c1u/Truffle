/**
 * @file      exception.h
 * @author    Rei Shimizu (shikugawa) <shikugawa@gmail.com>
 * @brief     Common exception class
 *
 * @copyright Copyright 2021 Rei Shimizu. All rights reserved.
 */

#ifndef TRUFFLE_EXCEPTION_H
#define TRUFFLE_EXCEPTION_H

#include <stdexcept>
#include <string>

namespace Truffle {

class TruffleException : public std::runtime_error {
 public:
  explicit TruffleException(const std::string& message)
      : std::runtime_error(message) {}
};

}  // namespace Truffle
#endif  // TRUFFLE_EXCEPTION_H
