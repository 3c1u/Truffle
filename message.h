//
// Created by 清水嶺 on 2021/01/01.
//

#ifndef TRUFFLE_MESSAGE_H
#define TRUFFLE_MESSAGE_H

#include <string>

namespace Truffle {
struct Message {
  // 宛先オブジェクト
  std::string dst_object;
  std::string detail;
};

}  // namespace Truffle
#endif  // TRUFFLE_MESSAGE_H
