/**
 * @file      metrics.h
 * @author    Rei Shimizu (shikugawa) <shikugawa@gmail.com>
 * @brief     Additional metrics components
 *
 * @copyright Copyright 2021 Rei Shimizu. All rights reserved.
 */

#ifndef TRUFFLE_METRICS_H
#define TRUFFLE_METRICS_H

#include <chrono>
#include <iostream>

#include "common/logger.h"
#include "common/singleton.h"

namespace Truffle {

using SteadyClock = std::chrono::steady_clock;
using SteadyClockTimePoint = SteadyClock::time_point;

class FpsMetrics : public MutableSingleton<FpsMetrics> {
 public:
  static void incFrame() { FpsMetrics::get().incFrame_(); }

  static bool shouldCalcFps() { return FpsMetrics::get().shouldCalcFps_(); }

  static double fps() { return FpsMetrics::get().fps_(); }

 private:
  friend class MutableSingleton<FpsMetrics>;

  // Ensure that this constructor called at startup time.
  explicit FpsMetrics() : start_time_(SteadyClock::now()) {}

  void incFrame_() {
    if (std::numeric_limits<uint64_t>::max() < frame_) {
      start_time_ = SteadyClock::now();
      frame_ = 1;
    } else {
      ++frame_;
    }
    ++calc_per_frames_;
  }

  // TODO(shikugawa): It should be provided with DispatcherMetricsDriver class
  bool shouldCalcFps_() {
    bool result = calc_per_frames_ == default_calc_per_frames_;
    if (result) {
      calc_per_frames_ = 1;
    }
    return result;
  }

  double fps_() {
    auto current_time = SteadyClock::now().time_since_epoch();
    return frame_ / std::chrono::duration_cast<std::chrono::seconds>(
                        current_time - start_time_.time_since_epoch())
                        .count();
  }

  SteadyClockTimePoint start_time_;
  uint64_t frame_ = 0;
  uint64_t calc_per_frames_ = 1;
  const uint64_t default_calc_per_frames_ = 100;
};

}  // namespace Truffle

#endif  // TRUFFLE_METRICS_H
