//
// Created by shikugawa on 2020/12/19.
//

#ifndef TRUFFLE_NON_COPYABLE_H
#define TRUFFLE_NON_COPYABLE_H

namespace Truffle {

class NonCopyable {
 protected:
  NonCopyable() = default;

  NonCopyable(NonCopyable&&) noexcept = delete;
  NonCopyable& operator=(NonCopyable&&) noexcept = delete;

  NonCopyable(const NonCopyable&) = delete;
  NonCopyable& operator=(const NonCopyable&) = delete;
};

}  // namespace Truffle
#endif  // Truffle_NON_COPYABLE_H
