#include <iostream>
#include <istream>
#include "CommandProcessor.h"
#include "../GameEngine/GameEngine.h"

void testCommandProcessor() {
  auto ge = new GameEngine();
  ge->setState(GameEngine::START);
  auto *cp = new CommandProcessor();
  for (int i = 0; i < 3; i++) {
      cp->getCommand();
  }

  auto list = cp->getCommandList();

  for (auto i : list) {
    std::cout << *i;
    cp->validate(i);
  }
}