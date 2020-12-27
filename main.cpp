#include <iostream>
#include <absl/strings/str_format.h>
#include "window.h"
#include "renderer.h"
#include "scene.h"
#include "dispatcher.h"
#include "texture.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

using Mirai::Window;
using Mirai::Renderer;
using Mirai::Dispatcher;
using Mirai::ImageTextureBehavior;
using Mirai::SceneManager;
using Mirai::Scene;
using Mirai::ScenePtr;

class Genji final : public ImageTextureBehavior {
 public:
  explicit Genji(Renderer& r) : ImageTextureBehavior("../testdata/genji.jpg", r) {}

  void start() override {
    std::cout << "start" << std::endl;
  }

  void update() override {
    render(0, 0);
  }

  void onKeyPressed(SDL_Event& ev) override {
    std::cout << "pressed" << std::endl;
  }
};

int main() {
  // TODO: provide on init manager
  int img_flags = IMG_INIT_PNG;
  if (!(IMG_Init(img_flags) & img_flags)) {
    return false;
  }

  // create window
  Window w;
  w.setWindowName("hoge");
  w.startWindow();

  // create renderer
  Renderer r;
  r.init(w);
  r.setDrawColor(0xff, 0xff, 0xff, 0xff);

  // create scene
  ScenePtr s1 = std::make_shared<Scene>("root_scene");
  Genji dot(r);
  s1->setBehavior(dot);

  // define scene manager
  SceneManager sm(s1);

  Dispatcher d(sm, r);
  d.run();

  return 0;
}
