//
// Created by 清水嶺 on 2021/01/03.
//

#ifndef TRUFFLE_FPS_H
#define TRUFFLE_FPS_H

#include "object_tree.h"
#include "texture.h"

namespace Truffle {

class FpsController : public SceneIsolatedTruffleControllerImpl {
 public:
  explicit FpsController(const Renderer& renderer, const Font& font,
                         std::string name)
      : SceneIsolatedTruffleControllerImpl(name),
        texture_(*this, renderer, font, name + "_texture", 0, 0) {}

  void update(SDL_Event& ev) final {
    Color cg{0xff, 0xff, 0xff, 0xff};
    texture_.loadBlendTexture("string", cg);
  }

 private:
  TextTexture texture_;
};

}  // namespace Truffle

#endif  // TRUFFLE_FPS_H
