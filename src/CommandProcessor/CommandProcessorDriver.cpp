#include <iostream>
#include <istream>
#include "CommandProcessor.h"
#include "../GameEngine/GameEngine.h"

void testCommandProcessor() {
    auto ge = new GameEngine();
    ge->setState(GameEngine::MAP_LOADED);
    cout << "Type in 3 commands to test.\nYou are currently in state " << GameEngine::toString(ge->getState())
         << endl;
//    auto *cp = new CommandProcessor();
//    for (int i = 0; i < 3; i++) {
//        cp->getCommand();
//    }

    auto *fcpa = new FileCommandProcessorAdapter("../src/CommandProcessor/cmds.txt");

    fcpa->getCommand();

    auto list = fcpa->getCommandList();

    for (auto i: list) {
        std::cout << *i;
    }
}