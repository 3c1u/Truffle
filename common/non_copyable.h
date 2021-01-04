/**
 * @file      non_copyable.h
 * @author    Rei Shimizu (shikugawa) <shikugawa@gmail.com>
 * @brief     Common interface to disable unexpected copy and move
 *
 * @copyright Copyright 2021 Rei Shimizu. All rights reserved.
 */

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

#endif  // TRUFFLE_NON_COPYABLE_H
