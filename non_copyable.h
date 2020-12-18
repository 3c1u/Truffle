//
// Created by shikugawa on 2020/12/19.
//

#ifndef MIRAI_NON_COPYABLE_H
#define MIRAI_NON_COPYABLE_H

namespace Mirai {

class NonCopyable {
 protected:
  NonCopyable() = default;

  NonCopyable(NonCopyable&&) noexcept = delete;
  NonCopyable& operator=(NonCopyable&&) noexcept = delete;

  NonCopyable(const NonCopyable&) = delete;
  NonCopyable& operator=(const NonCopyable&) = delete;
};

}  // namespace Mirai
#endif  // Mirai_NON_COPYABLE_H
