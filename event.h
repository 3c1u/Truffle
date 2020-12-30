//
// Created by shikugawa on 2020/12/29.
//

#ifndef TRUFFLE_EVENT_H
#define TRUFFLE_EVENT_H

namespace Truffle {

/**
 * マウスが指定領域にホバーした場合に発行されるイベントを識別するためのコード
 */
static constexpr int MOUSE_HOVERED = 0;

/**
 * マウスが指定領域外にある時に発行されるイベントを識別するためのコード
 */
static constexpr int MOUSE_UNHOVERED = 1;

/**
 * シーンの変更イベントを識別するためのコード
 */
static constexpr int SCENE_CHANGED = 3;
}  // namespace Truffle

#endif  // TRUFFLE_EVENT_H
