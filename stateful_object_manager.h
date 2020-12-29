//
// Created by shikugawa on 2020/12/28.
//

#ifndef TRUFFLE_STATEFUL_OBJECT_MANAGER_H
#define TRUFFLE_STATEFUL_OBJECT_MANAGER_H

#include <memory>
#include <unordered_map>

#include "exception.h"
#include "logger.h"

namespace Truffle {

// ステートレスである際にStatefulObjectManagerに与えるステート定義
enum class NullState {};

template <class StatefulObject, class State>
class StatefulObjectManager {
 public:
  StatefulObjectManager() {}

  void setInitStatefulObject(State init, std::shared_ptr<StatefulObject> obj) {
    if (init_) {
      throw TruffleException("init stateful object can't be called twice");
    }
    if (binded_stateful_object_.find(init) != binded_stateful_object_.end()) {
      return;
    }
    bindStatefulObject(init, obj);
    current_state_ = init;
    prev_state_object_ = binded_stateful_object_.at(init);
    init_ = true;
  }

  void bindStatefulObject(State s, std::shared_ptr<StatefulObject> obj) {
    binded_stateful_object_[s] = obj;
  }

  void defineStateTransition(State from, State to) {
    auto src_state = transition_table_.find(from);
    if (src_state == transition_table_.end()) {
      transition_table_[from] = std::set<State>();
    }
    transition_table_[from].insert(to);
  }

  void stateTransition(State to) {
    if (!init_) {
      throw TruffleException("StateMachine doesn't be initialized");
    }
    assert(transition_table_.find(current_state_) != transition_table_.end());
    std::unique_lock<std::mutex> l(mux_);
    if (transition_table_[current_state_].find(to) ==
        transition_table_[current_state_].end()) {
      throw TruffleException("Can't execute unregistered transition");
    }
    if (binded_stateful_object_.find(current_state_) !=
        binded_stateful_object_.end()) {
      prev_state_object_ = binded_stateful_object_.at(current_state_);
    } else {
      log(LogLevel::INFO,
          "previous StatefulObject can't be loaded because current state has "
          "no StatefulObject");
    }
    current_state_ = to;
  }

  StatefulObject& activeStateObject() {
    if (!init_) {
      throw TruffleException("StateMachine doesn't be initialized");
    }
    auto current_obj = binded_stateful_object_.find(current_state_);
    if (current_obj == binded_stateful_object_.end()) {
      // 状態に一致するStatefulObjectがなければ、現在の状態になる前の状態におけるStatefulObjectを返却する。
      // 初期状態においては必ずStatefulObjectが存在することが保証されているので、最大で初期状態のStatefulObjectの参照を返却する。
      assert(prev_state_object_ != nullptr);
      return *prev_state_object_;
    }
    return *binded_stateful_object_.at(current_state_);
  }

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

template <class StatelessObject>
class StatefulObjectManager<StatelessObject, NullState> {
 public:
  StatefulObjectManager() {}

  void setInitStatefulObject(std::shared_ptr<StatelessObject> obj) {
    if (init_) {
      throw TruffleException("init stateful object can't be called twice");
    }
    active_object_ = obj;
    init_ = true;
  }

  StatelessObject& activeStateObject() {
    if (!init_) {
      throw TruffleException("StateMachine doesn't be initialized");
    }
    return *active_object_.get();
  }

 private:
  std::shared_ptr<StatelessObject> active_object_;
  bool init_ = false;
  std::mutex mux_;
};

}  // namespace Truffle

#endif  // TRUFFLE_STATEFUL_OBJECT_MANAGER_H
