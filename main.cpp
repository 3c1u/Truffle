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
using Truffle::SceneManager;
using Truffle::Scene;
using Truffle::ScenePtr;
using Truffle::Color;
using Truffle::ImageButton;
using Truffle::NullState;
using Truffle::Font;
using Truffle::TextTextureMode;
using Truffle::StatefulObjectManager;
using Truffle::TruffleBehavior;
using Truffle::TextTexture;
using Truffle::ButtonState;
using Truffle::ImageTextureFactory;

class Genji final : public TruffleBehavior {
 public:
  static constexpr std::string_view name = "genji_behavior";

  explicit Genji(Renderer& r)
    : TruffleBehavior(name.data()),
      texture_(r, "../testdata/genji.jpg", name.data(), 0, 0) {
      renderables.emplace_front(texture_);
  }

  void start() override {
    std::cout << "start" << std::endl;
  }

  void update(SDL_Event& ev) override {
      if (ev.type == SDL_KEYDOWN) {
          std::cout << "keydown" << std::endl;
      }
  }

private:
    ImageTexture texture_;
};

enum class IllustyaState {
    Normal,
    Hovered
};

class Illustya final : public TruffleBehavior {
public:
    static constexpr std::string_view name = "illustya_behavior";

    explicit Illustya(Renderer& r) : TruffleBehavior(name.data()), factory_(r) {
        // Define state machine
        state_manager_.setInitStatefulObject(
                IllustyaState::Normal,
                factory_.create("../testdata/home.png", name.data(), 0, 0));
        state_manager_.bindStatefulObject(
                IllustyaState::Hovered,
                factory_.create("../testdata/top.png", name.data(), 50, 0));
        state_manager_.setStateTransition(IllustyaState::Normal, IllustyaState::Hovered);
        state_manager_.setStateTransition(IllustyaState::Hovered, IllustyaState::Normal);

        // register objects as renderable
        auto& home = state_manager_.statefulObject(IllustyaState::Normal);
        renderables.emplace_front(home);
        auto& top = state_manager_.statefulObject(IllustyaState::Hovered);
        top.disableRender();
        renderables.emplace_front(top);
    }

    void start() override {
        std::cout << "start" << std::endl;
    }

    void update(SDL_Event& ev) override {
        if (ev.type == SDL_KEYDOWN && state_manager_.activeState() == IllustyaState::Normal) {
            state_manager_.stateTransition(IllustyaState::Hovered);
            state_manager_.statefulObject(IllustyaState::Normal).disableRender();
            state_manager_.statefulObject(IllustyaState::Hovered).enableRender();
        } else if (ev.type == SDL_KEYUP && state_manager_.activeState() == IllustyaState::Hovered) {
            state_manager_.stateTransition(IllustyaState::Normal);
            state_manager_.statefulObject(IllustyaState::Hovered).disableRender();
            state_manager_.statefulObject(IllustyaState::Normal).enableRender();
        }
    }

private:
    ImageTextureFactory factory_;
    StatefulObjectManager<ImageTexture, IllustyaState> state_manager_;
};

class TimeBoard : public TruffleBehavior {
public:
    static constexpr std::string_view name = "genji_behavior";

    explicit TimeBoard(Renderer& r, Font& f)
        : TruffleBehavior(name.data()), texture_(r, f, name.data(), 0, 0) {
        renderables.emplace_front(texture_);
    }

    void start() override {
        auto color = Color { 0x00, 0x00, 0x00, 0xff};
        texture_.loadBlendTexture("0:00", color);
    }

    void update(SDL_Event& ev) override {
    }

private:
    TextTexture texture_;
};

using Truffle::SceneState;

class ImageButton2 : public ImageButton {
 public:
   ImageButton2(SceneManager<SceneState>& manager,
               Renderer& r, std::string name, int x, int y,
               std::string path1, std::string path2)
      : ImageButton(r, name, x, y, path1, path2), manager_(manager) {}

   void onMouseHovered() override final {
     state_manager.stateTransition(ButtonState::Hovered);
     std::cout << "imagebutton2 hovered" << std::endl;
   }

   void onButtonPressed() override final {
//     assert(manager_.currentSceneState() == SceneState::Init);
     manager_.transitScene(SceneState::Clicked);
   }

  private:
   SceneManager<SceneState>& manager_;
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

  // Load font
  Font f("../font/lazy.ttf", 100);

  // define scene manager
  SceneManager<SceneState> manager;
  // create scene
  auto& s1 = manager.addScene(SceneState::Init, "root_scene");
  TimeBoard tb(r, f);
  s1.setBehavior(tb);
  Illustya it(r);
  s1.setBehavior(it);
  Genji dot(r);
  s1.setBehavior(dot);
  ImageButton2 ib(manager, r, "illustya", 150, 150, "../testdata/home.png", "../testdata/top.png");
  s1.setButton(ib);

  auto& s2 = manager.addScene(SceneState::Clicked, "clicked_scene");
  TimeBoard tb3(r, f);
  s2.setBehavior(tb3);
  ImageButton2 i2b(manager, r, "illustya", 150, 150, "../testdata/home.png", "../testdata/top.png");
  s2.setButton(i2b);

  manager.setSceneTransition(SceneState::Init, SceneState::Clicked);

  Dispatcher d(manager, r);
  d.run();

  SDL_Quit();
  return 0;
}
