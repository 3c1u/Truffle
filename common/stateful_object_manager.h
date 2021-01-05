/**
 * @file      stateful_object_manager.h
 * @author    Rei Shimizu (shikugawa) <shikugawa@gmail.com>
 * @brief     Manage stateful object with unified interface
 *
 * @copyright Copyright 2021 Rei Shimizu. All rights reserved.
 */

#ifndef TRUFFLE_STATEFUL_OBJECT_MANAGER_H
#define TRUFFLE_STATEFUL_OBJECT_MANAGER_H

#include <mutex>
#include <set>

#include "exception.h"
#include "logger.h"
#include "non_copyable.h"

namespace Truffle {

// ステートレスである際にStatefulObjectManagerに与えるステート定義
enum class NullState {};

/**
 * 状態遷移及び状態遷移に伴うオブジェクトの変化をハンドリングするクラス
 *
 * @tparam StatefulObject
 * @tparam State
 */
template <class StatefulObject, class State>
class StatefulObjectManager : NonCopyable {
 public:
  StatefulObjectManager() = default;

  /**
   * 初期状態が定義されたか否か
   *
   * @return
   */
  bool initialized() { return init_; }

  /**
   * ステートマシンにおける初期状態を定義
   *
   * @param init 初期状態を示す状態
   * @param obj 初期状態を示すオブジェクト
   */
  template <typename... Args>
  void setInitStatefulObject(State init, Args&&... args) {
    if (init_) {
      throw TruffleException("init stateful object can't be called twice");
    }
    if (binded_stateful_object_.find(init) != binded_stateful_object_.end()) {
      return;
    }
    bindStatefulObject(init, std::forward<Args>(args)...);
    current_state_ = init;
    prev_state_object_ = binded_stateful_object_.at(init);
    init_ = true;
  }

  /**
   * 状態とオブジェクトを紐付ける
   *
   * @param state 紐付けたい状態
   * @param obj 紐付けたいオブジェクト
   */
  template <typename... Args>
  void bindStatefulObject(State state, Args&&... args) {
    binded_stateful_object_[state] = std::make_shared<StatefulObject>(args...);
  }

  /**
   * 状態遷移の定義
   *
   * @param from ソース状態
   * @param to 対象状態
   */
  void setStateTransition(State from, State to) {
    auto src_state = transition_table_.find(from);
    if (src_state == transition_table_.end()) {
      transition_table_[from] = std::set<State>();
    }
    transition_table_[from].insert(to);
  }

  /**
   * 状態遷移を行う
   *
   * @param to 対象状態
   */
  void stateTransition(State to) {
    if (!init_) {
      throw TruffleException("StateMachine doesn't be initialized");
    }
    assert(transition_table_.find(current_state_) != transition_table_.end());
    {
      std::unique_lock<std::mutex> l(mux_);
      if (transition_table_[current_state_].find(to) ==
          transition_table_[current_state_].end()) {
        throw TruffleException("Can't execute unregistered transition");
      }
      if (binded_stateful_object_.find(current_state_) !=
          binded_stateful_object_.end()) {
        prev_state_object_ = binded_stateful_object_.at(current_state_);
      } else {
        Logger::log(LogLevel::INFO,
                   "previous StatefulObject can't be loaded because current
                   state has " "no StatefulObject");
      }
      current_state_ = to;
    }
  }

  /**
   * 現在の状態におけるオブジェクトを取得する
   */
  StatefulObject& activeStateObject() {
    if (!init_) {
      throw TruffleException("StateMachine doesn't be initialized");
    }
    std::unique_lock<std::mutex> l(mux_);
    auto current_obj = binded_stateful_object_.find(current_state_);
    if (current_obj == binded_stateful_object_.end()) {
      /**
       * 状態に一致するStatefulObjectがなければ、現在の状態になる前の状態におけるStatefulObjectを返却する。
       * 初期状態においては必ずStatefulObjectが存在することが保証されているので、最大で初期状態のStatefulObjectの参照を返却する。
       */
      assert(prev_state_object_ != nullptr);
      return *prev_state_object_;
    }
    return *binded_stateful_object_.at(current_state_);
  }

  /**
   * 与えられた状態に対応するオブジェクトを取得する。もし対応するオブジェクトがなければ例外を返す。
   *
   * @param state 状態
   * @return
   */
  StatefulObject& statefulObject(State state) {
    std::unique_lock<std::mutex> l(mux_);
    if (binded_stateful_object_.find(state) == binded_stateful_object_.end()) {
      throw TruffleException("Can't retrieve corresponding object");
    }
    return *binded_stateful_object_.at(state);
  }

  /**
   * Managerの管理対象となるすべてのオブジェクトを返す
   * @return
   */
  std::unordered_map<State, std::shared_ptr<StatefulObject>>&
  allManagedStatefulObject() & {
    return binded_stateful_object_;
  }
  [[nodiscard]] const std::unordered_map<State,
                                         std::shared_ptr<StatefulObject>>&
  allManagedStatefulObject() const& {
    return binded_stateful_object_;
  }

  /**
   * 現在の状態を取得する
   */
  State activeState() { return current_state_; }

 private:
  // 過去のStatefulObjectが存在していた状態におけるStatefulObjectの参照を保持する。
  std::shared_ptr<StatefulObject> prev_state_object_;
  State current_state_;
  std::unordered_map<State, std::shared_ptr<StatefulObject>>
      binded_stateful_object_;
  std::unordered_map<State, std::set<State>> transition_table_;
  bool init_ = false;
  std::mutex mux_;
};

/**
 * 状態が遷移しない場合の特殊化クラス
 *
 * @tparam StatelessObject
 */
template <class StatelessObject>
class StatefulObjectManager<StatelessObject, NullState> {
 public:
  StatefulObjectManager() = default;

  /**
   * ステートマシンにおける初期状態を定義
   *
   * @param init 初期状態を示す状態
   * @param obj 初期状態を示すオブジェクト
   */
  template <typename... Args>
  void setInitStatefulObject(Args&&... args) {
    if (init_) {
      throw TruffleException("init stateful object can't be called twice");
    }
    active_object_ = std::make_shared<StatelessObject>(args...);
    init_ = true;
  }

  /**
   * 現在の状態におけるオブジェクトを取得する
   */
  StatelessObject& activeStateObject() {
    if (!init_) {
      throw TruffleException("StateMachine doesn't be initialized");
    }
    return *active_object_.get();
  }

 private:
  std::shared_ptr<StatelessObject> active_object_;
  bool init_ = false;
};

}  // namespace Truffle

#endif  // TRUFFLE_STATEFUL_OBJECT_MANAGER_H
