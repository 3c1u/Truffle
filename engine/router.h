/**
 * @file      router.h
 * @author    Rei Shimizu (shikugawa) <shikugawa@gmail.com>
 * @brief     Routing message between actor
 *
 * @copyright Copyright 2021 Rei Shimizu. All rights reserved.
 */

#ifndef TRUFFLE_ROUTER_H
#define TRUFFLE_ROUTER_H

#include "actor_table.h"
#include "common/singleton.h"

namespace Truffle {

class Router : public ConstSingleton<Router> {
 public:
  template <class T>
  bool transport(Address address, T&& message) {
    return Router::get().transport_(address, std::forward<T>(message));
  }

 private:
  friend class ConstSingleton<Router>;

  Router() = default;

  template <class T>
  bool transport_(Address address, T&& message);
};

template <class T>
bool Router::transport_(Address address, T&& message) {
  if (!RouteTable::get().lookup(address)) {
    return false;
  }
}

}  // namespace Truffle

#endif  // TRUFFLE_ROUTER_H
