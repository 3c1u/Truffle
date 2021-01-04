/**
 * @file      event.h
 * @author    Rei Shimizu (shikugawa) <shikugawa@gmail.com>
 * @brief     Truffle custom event code
 *
 * @copyright Copyright 2021 Rei Shimizu. All rights reserved.
 */

#ifndef TRUFFLE_EVENT_H
#define TRUFFLE_EVENT_H

#include <SDL2/SDL.h>
#include <stdint.h>

#include <functional>

namespace Truffle {

using CustomEventCallback = std::function<void(SDL_Event&)>;

/**
 * シーンの変更イベントを識別するためのコード
 */
static constexpr uint32_t EV_SCENE_CHANGED = 0;

}  // namespace Truffle

#endif  // TRUFFLE_EVENT_H
