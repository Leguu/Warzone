#include <iostream>
#include <istream>
#include "CommandProcessor.h"
#include "../GameEngine/GameEngine.h"

void testCommandProcessor() {
    auto ge = new GameEngine();
    ge->setState(GameEngine::MAP_LOADED);
    cout << "Type in commands to test.\nYou are currently in state " << GameEngine::toString(ge->getState())
         << endl;
//    auto *cp = new CommandProcessor();
//    for (int i = 0; i < 3; i++) {
//        cp->getCommand();
//    }

    string path = "../src/CommandProcessor/cmds.txt";
    auto *fcpa = new FileCommandProcessorAdapter(path);
    fcpa->flr->ifile.open(path);

    while(!fcpa->flr->ifile.eof()) {

        auto input = Utils::getInputString("You are taking commands from a file now.\n"
                                           "Type Y to read from line or N to exit.");

        if (input == "Y") {
            if(fcpa->getCommand()) {
                cout << "Command correct in this line" << endl;
            } else {
                cout << "Command invalid in this line" << endl;
            }
            cout << "------------" << endl;
            auto list = fcpa->getCommandList();
            for (auto i: list) {
                std::cout << *i;
            }
            cout << "------------" << endl;
        }

        else if (input == "N")
            break;
        else cout << "invalid command. Y or N only";
    }



}