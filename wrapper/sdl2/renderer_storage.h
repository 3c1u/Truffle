/**
 * @file      renderer_storage.h
 * @author    Rei Shimizu (shikugawa) <shikugawa@gmail.com>
 * @brief
 *
 * @copyright Copyright 2021 Rei Shimizu. All rights reserved.
 */

#ifndef TRUFFLE_RENDERER_STORAGE_H
#define TRUFFLE_RENDERER_STORAGE_H

#include <memory>
#include <utility>

#include "common/non_copyable.h"
#include "common/singleton.h"
#include "wrapper/sdl2/renderer.h"
#include "wrapper/sdl2/window.h"

namespace Truffle {

class RendererStorage : public MutableSingleton<RendererStorage>, NonCopyable {
 public:
  static std::shared_ptr<Renderer> activeRenderer() {
    return RendererStorage::get().activeRenderer_();
  }

  static void activateRenderer(const Window& window) {
    RendererStorage::get().activateRenderer_(window);
  }

 private:
  friend class MutableSingleton<RendererStorage>;

  RendererStorage() = default;

  std::shared_ptr<Renderer> activeRenderer_() {
    assert(renderer_ != nullptr);
    return renderer_;
  }

  void activateRenderer_(const Window& window) {
    if (!renderer_) {
      renderer_ = std::shared_ptr<Renderer>(&Renderer::get(window));
    }
  }

  std::shared_ptr<Renderer> renderer_;
};

}  // namespace Truffle

#endif  // TRUFFLE_RENDERER_STORAGE_H
