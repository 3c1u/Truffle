/**
 * @file      engine_config.h
 * @author    Rei Shimizu (shikugawa) <shikugawa@gmail.com>
 * @brief     Configurations of Truffle engine initializer
 *
 * @copyright Copyright 2021 Rei Shimizu. All rights reserved.
 */

#ifndef TRUFFLE_ENGINE_CONFIG_H
#define TRUFFLE_ENGINE_CONFIG_H

#include <string>
#include <utility>

#include "wrapper/sdl2/color.h"

namespace Truffle {

struct EngineConfig {
  int window_height;
  int window_width;
  std::string name;
  Color renderer_color;
  std::vector<std::pair<std::string, std::string>> font_paths;
};

}  // namespace Truffle

#endif  // TRUFFLE_ENGINE_CONFIG_H
