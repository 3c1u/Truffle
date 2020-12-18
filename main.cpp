#include <iostream>
#include <absl/strings/str_format.h>
#include "window.h"
#include "renderer.h"
#include "dispatcher.h"

using Mirai::Window;
using Mirai::Renderer;
using Mirai::Dispatcher;

int main() {
  Window w;
  w.setWindowName("hoge");
  w.startWindow();

  Renderer r;
  r.init(w);
  r.setDrawColor(0xff, 0xff, 0xff, 0xff);

  Dispatcher d;

  d.run();

  return 0;
}
