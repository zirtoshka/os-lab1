#include <iostream>

#include "monolith/HelloWorld.hpp"
#include "monolith/Shell.hpp"

namespace monolith::app {

void Main() {
  std::cout << HelloWorld() << '\n';
  ShellRun();
}

}  // namespace monolith::app

int main() {
  monolith::app::Main();
}
