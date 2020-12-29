#include <iostream>
#include <absl/strings/str_format.h>
#include "window.h"
#include "renderer.h"
#include "scene.h"
#include "dispatcher.h"
#include "texture.h"
#include "button.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

using Truffle::Window;
using Truffle::Renderer;
using Truffle::Dispatcher;
using Truffle::ImageTexture;
using Truffle::ImageTextureBehavior;
using Truffle::SceneManager;
using Truffle::Scene;
using Truffle::ScenePtr;
using Truffle::Color;
using Truffle::ImageButton;
using Truffle::NullState;

class Genji final : public ImageTextureBehavior<NullState> /* stateless */ {
 public:
  static constexpr std::string_view name = "genji_behavior";

  explicit Genji(Renderer& r) : ImageTextureBehavior(r, name.data(), 0, 0) {
    setInitTexture("../testdata/genji.jpg", name.data());
  }

  void start() override {
    std::cout << "start" << std::endl;
  }

  void update(SDL_Event& ev) override {
      if (ev.type == SDL_KEYDOWN) {
          std::cout << "keydown" << std::endl;
      }
  }
};

enum class IllustyaState {
    Normal,
    Hovered
};

class Illustya final : public ImageTextureBehavior<IllustyaState> {
public:
    static constexpr std::string_view name = "genji_behavior";

    explicit Illustya(Renderer& r) : ImageTextureBehavior(r, name.data(), 0, 0) {
        setInitTexture(IllustyaState::Normal,
                       std::make_shared<ImageTexture>(r, "../testdata/genji.jpg", name.data()));
    }

    void start() override {
        std::cout << "start" << std::endl;
    }

    void update(SDL_Event& ev) override {
        if (ev.type == SDL_KEYDOWN) {
            std::cout << "keydown" << std::endl;
        }
    }
};

int main() {
    // TODO: provide on init manager
  int img_flags = IMG_INIT_PNG;
  if (!(IMG_Init(img_flags) & img_flags)) {
    return 1;
  }
  // Initialize font
  if (TTF_Init() == -1) {
      return 1;
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
//  ImageButton ib(r, "illustya", 150, 150, "../testdata/genji.jpg");
//  s1->setButton(ib);

  // define scene manager
  SceneManager sm(s1);

  Dispatcher d(sm, r);
  d.run();

  return 0;
}
