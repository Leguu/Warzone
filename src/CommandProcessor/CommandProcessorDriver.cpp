#include <iostream>
#include <istream>
#include "CommandProcessor.h"
#include "../GameEngine/GameEngine.h"

void testCommandProcessor() {
  auto ge = new GameEngine();
  auto *cp = new CommandProcessor();
  cp->getCommand();
  auto list = cp->getCommandList();

  for (auto i : list) {
    std::cout << *i;
    cp->validate(i);
  }
}