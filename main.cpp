#include <iostream>
#include <absl/strings/str_format.h>
#include "window.h"
#include "renderer.h"
#include "scene.h"
#include "dispatcher.h"
#include "texture.h"
#include "button.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

using Truffle::Window;
using Truffle::Renderer;
using Truffle::Dispatcher;
using Truffle::StaticImageTextureBehavior;
using Truffle::SceneManager;
using Truffle::Scene;
using Truffle::ScenePtr;
using Truffle::FixedButton;
using Truffle::Color;

class Genji final : public StaticImageTextureBehavior {
 public:
  static constexpr std::string_view name = "genji_behavior";

  explicit Genji(Renderer& r) : StaticImageTextureBehavior(r, "../testdata/genji.jpg", name.data(), 0, 0) {}

  void start() override {
    std::cout << "start" << std::endl;
  }

  void update() override {
    render();
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
//  FixedButton fb(r, "button1", 50, 50, 50, 50,
//                 Color {0x00, 0xff, 0x00, 0xff},
//                 [] { std::cout << "clicked" << std::endl; });
//  s1->setBehavior(fb);

  // define scene manager
  SceneManager sm(s1);

  Dispatcher d(sm, r);
  d.run();

  return 0;
}
