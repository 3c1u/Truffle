/**
 * @file      context.h
 * @author    Rei Shimizu (shikugawa) <shikugawa@gmail.com>
 * @brief     It holds global context, such as global controller / object
 *
 * @copyright Copyright 2021 Rei Shimizu. All rights reserved.
 */

#ifndef TRUFFLE_CONTEXT_H
#define TRUFFLE_CONTEXT_H

#include <absl/container/flat_hash_map.h>
#include <absl/strings/str_format.h>

#include "common/exception.h"
#include "common/singleton.h"
#include "controller.h"

namespace Truffle {

class IsolatedControllerStore
    : public MutableSingleton<IsolatedControllerStore> {
 public:
  static void set(TruffleController& controller) {
    IsolatedControllerStore::get().set_(controller);
  }

 private:
  friend class MutableSingleton<IsolatedControllerStore>;

  explicit IsolatedControllerStore() = default;

  void set_(TruffleController& controller);

  absl::flat_hash_map<std::string, TruffleControllerRef> controllers_;
};

class Context : public MutableSingleton<Context> {
 public:
  static void setController(TruffleController& controller) {
    Context::get().setController_(controller);
  }

 private:
  friend class MutableSingleton<Context>;

  void setController_(TruffleController& controller);

  explicit Context() = default;
};

}  // namespace Truffle

#endif  // TRUFFLE_CONTEXT_H
