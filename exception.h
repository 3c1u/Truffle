//
// Created by shikugawa on 2020/12/19.
//

#ifndef TRUFFLE_EXCEPTION_H
#define TRUFFLE_EXCEPTION_H

#include <exception>
#include <stdexcept>
#include <string>

namespace Truffle {
class TruffleException : public std::runtime_error {
 public:
  explicit TruffleException(const std::string& message)
      : std::runtime_error(message) {}
};

}  // namespace Truffle
#endif  // Truffle_EXCEPTION_H
