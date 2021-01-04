/**
 * @file      singleton.h
 * @author    Rei Shimizu (shikugawa) <shikugawa@gmail.com>
 * @brief     Base classes to achieve singleton class
 *
 * @copyright Copyright 2021 Rei Shimizu. All rights reserved.
 */

#ifndef TRUFFLE_SINGLETON_H
#define TRUFFLE_SINGLETON_H

#include <utility>

template <class T>
class ConstSingleton {
 public:
  template <typename... Args>
  static const T& get(Args&&... args) {
    static T* instance_ = new T(std::forward<Args>(args)...);
    return *instance_;
  }
};

template <class T>
class MutableSingleton {
 public:
  template <typename... Args>
  static T& get(Args&&... args) {
    static T* instance_ = new T(std::forward<Args>(args)...);
    return *instance_;
  }
};

#endif  // TRUFFLE_SINGLETON_H
