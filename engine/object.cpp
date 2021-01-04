/**
 * @file      object.cpp
 * @author    Rei Shimizu (shikugawa) <shikugawa@gmail.com>
 * @brief     The definition of object
 *
 * @copyright Copyright 2021 Rei Shimizu. All rights reserved.
 */

#include "object.h"

namespace Truffle {

TruffleObject::TruffleObject(std::string name) : name_(name) {}

void TruffleObject::setPoint(int x, int y) {
  render_rect.x = x;
  render_rect.y = y;
}

}  // namespace Truffle
