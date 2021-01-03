#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <absl/strings/str_format.h>

#include <iostream>

#include "button.h"
#include "dispatcher.h"
#include "font_storage.h"
#include "fps.h"
#include "logger.h"
#include "message.h"
#include "renderer.h"
#include "scene_manager.h"
#include "texture.h"
#include "window.h"

using Truffle::BasicButtonObject;
using Truffle::ButtonState;
using Truffle::Color;
using Truffle::Dispatcher;
using Truffle::Font;
using Truffle::FontStorage;
using Truffle::ImageTexture;
using Truffle::Message;
using Truffle::NullState;
using Truffle::Renderer;
using Truffle::SceneManager;
using Truffle::StatefulObjectManager;
using Truffle::TextTexture;
using Truffle::TextTextureMode;
using Truffle::TruffleControllerImpl;
using Truffle::TruffleScene;
using Truffle::Window;

// class Genji final : public TruffleControllerImpl {
// public:
//  static constexpr std::string_view name = "genji_behavior";
//
//  explicit Genji(Scene& parent_scene, const Renderer& r)
//      : TruffleControllerImpl(parent_scene, name.data()),
//        texture_(r, "../testdata/genji.jpg", name.data(), 0, 0) {
//    addRenderable(texture_);
//  }
//
//  void start() override { std::cout << "start" << std::endl; }
//
//  void update(SDL_Event& ev) override {
//    if (ev.type == SDL_KEYDOWN) {
//      std::cout << "keydown" << std::endl;
//    }
//  }
//
// private:
//  ImageTexture texture_;
//};
//
// enum class IllustyaState { Normal, Hovered };
//
// class Illustya final : public TruffleControllerImpl {
// public:
//  static constexpr std::string_view name = "illustya_behavior";
//
//  explicit Illustya(Scene& parent_scene, const Renderer& r)
//      : TruffleControllerImpl(parent_scene, name.data()) {
//    // Define state machine
//    state_manager_.setInitStatefulObject(
//        IllustyaState::Normal, r, "../testdata/home.png", name.data(), 0, 0);
//    state_manager_.bindStatefulObject(
//        IllustyaState::Hovered, r, "../testdata/top.png", name.data(), 50, 0);
//    state_manager_.setStateTransition(IllustyaState::Normal,
//                                      IllustyaState::Hovered);
//    state_manager_.setStateTransition(IllustyaState::Hovered,
//                                      IllustyaState::Normal);
//
//    // register objects as renderable
//    auto& home = state_manager_.statefulObject(IllustyaState::Normal);
//    addRenderable(home);
//    auto& top = state_manager_.statefulObject(IllustyaState::Hovered);
//    top.disableRender();
//    addRenderable(top);
//  }
//
//  void start() override { std::cout << "start" << std::endl; }
//
//  void update(SDL_Event& ev) override {
//    if (ev.type == SDL_KEYDOWN &&
//        state_manager_.activeState() == IllustyaState::Normal) {
//      state_manager_.stateTransition(IllustyaState::Hovered);
//      state_manager_.statefulObject(IllustyaState::Normal).disableRender();
//      state_manager_.statefulObject(IllustyaState::Hovered).enableRender();
//    } else if (ev.type == SDL_KEYUP &&
//               state_manager_.activeState() == IllustyaState::Hovered) {
//      state_manager_.stateTransition(IllustyaState::Normal);
//      state_manager_.statefulObject(IllustyaState::Hovered).disableRender();
//      state_manager_.statefulObject(IllustyaState::Normal).enableRender();
//    }
//  }
//
// private:
//  StatefulObjectManager<ImageTexture, IllustyaState> state_manager_;
//};

enum class SceneState {
  Init,
  Clicked,
};

// class BasicButtonObject2 : public BasicButtonObject {
// public:
//  static constexpr std::string_view name = "image_button2";
//
//  BasicButtonObject2(SceneManager<SceneState>& manager,
//               Renderer& r, int x, int y, std::string path1, std::string
//               path2)
//      : BasicButtonObject(r, name.data(), x, y, path1, path2),
//        manager_(manager) {}
//
//  void onMouseHovered() override final {
//    state_manager.stateTransition(ButtonState::Hovered);
//    std::cout << "BasicButtonObject2 hovered" << std::endl;
//  }
//
//  void onButtonPressed() override final {
//    sendMessage("counter_behavior", Message{"message"});
//  }
//
// private:
//  SceneManager<SceneState>& manager_;
//};

class CounterController : public TruffleControllerImpl {
 public:
  static constexpr std::string_view name = "counter_behavior";

  explicit CounterController(TruffleScene& scene, const Renderer& r,
                             const Font& f)
      : TruffleControllerImpl(scene, name.data()),
        texture_(*this, r, f, "counter_board", 0, 0),
        button_(*this, r, "counter_button", 150, 150, "../testdata/home.png",
                "../testdata/top.png") {
    addObject(texture_);
    addObject(button_);
  }

  void start() override final {
    auto color = Color{0x00, 0x00, 0x00, 0xff};
    texture_.loadBlendTexture(current_text_, color);
  }

  void update(SDL_Event& ev) override final {
    auto message = recvMessage();
    if (message.has_value()) {
      auto current_num = std::stoi(current_text_);
      ++current_num;
      current_text_ = std::to_string(current_num);
      auto color = Color{0x00, 0x00, 0x00, 0xff};
      texture_.loadBlendTexture(current_text_, color);
    }
  }

 private:
  std::string current_text_ = "0";
  TextTexture texture_;
  BasicButtonObject button_;
};

int main() {
  // TODO: provide on init manager
  int img_flags = IMG_INIT_PNG;
  if (!(IMG_Init(img_flags) & img_flags)) {
    return 1;
  }

  // create window
  const auto& window = Window::get("window name", 680, 480);

  // create renderer
  auto& renderer = Renderer::get(window);
  renderer.setDrawColor(Color{0xff, 0xff, 0xff, 0xff});

  // Load font
  std::unique_ptr<Font> f{};
  try {
    f = FontStorage::openFont("../font/lazy.ttf", 100);
  } catch (Truffle::TruffleException const& e) {
    Truffle::log(Truffle::LogLevel::ERROR,
                 fmt::format("FontStorage::openFont failed: {}", e.what()));
    return 1;
  }

  // define scene manager
  SceneManager<SceneState> manager;
  // create scene
  auto& s1 = manager.addScene(SceneState::Init, "root_scene");
  // Add counter
  CounterController tb(s1, renderer, *f);

  //  Illustya it(renderer);
  //  s1.setBehavior(it);
  //  Genji dot(renderer);
  //  s1.setBehavior(dot);

  //  s1.setButton(ib);
  //
  //  auto& s2 = manager.addScene(SceneState::Clicked, "clicked_scene");
  //  TimeBoard tb3(renderer, f);
  //  s2.setBehavior(tb3);
  //  BasicButtonObject2 i2b(manager, renderer, "illustya", 150, 150,
  //  "../testdata/home.png", "../testdata/top.png"); s2.setButton(i2b);
  //
  //  manager.setSceneTransition(SceneState::Init, SceneState::Clicked);

  Dispatcher d(manager, renderer);
  d.run();

  SDL_Quit();
  return 0;
}
