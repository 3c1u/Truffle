//
// Created by 清水嶺 on 2020/12/31.
//

#ifndef TRUFFLE_SINGLETON_H
#define TRUFFLE_SINGLETON_H

#include <memory>

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
