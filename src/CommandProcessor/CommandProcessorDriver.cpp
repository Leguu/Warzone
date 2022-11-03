#include <iostream>
#include <istream>
#include "CommandProcessor.h"
#include "../GameEngine/GameEngine.h"

void testCommandProcessor() {
    auto ge = new GameEngine();
    ge->setState(GameEngine::MAP_LOADED);
    cout << "You are currently in state " << GameEngine::gameStates[ge->getState()] << endl;


    while (true) {
        auto input1 = Utils::getInputString("Where do you want to receive commands?\n"
                                            "Enter either file or console. Type leave to go back to previous menu.");
        if (input1 == "console") {
            cout << "You are in console mode. Type your commands." << endl;
            auto *cp = new CommandProcessor();
            cp->getCommand();
            auto list = cp->getCommandList();
            for (auto i: list) {
                std::cout << *i;
            }
        } else if (input1 == "file") {
            cout << "You are in file read mode." << endl;
            string fileName = Utils::getInputString("Enter the name of the text file you want to read");
            string path = "../src/CommandProcessor/" + fileName;
            auto fcpa = new FileCommandProcessorAdapter(path);
            fcpa->flr->ifile.open(path);
            cout << "You are taking commands from a file now." << endl;
            while (!fcpa->flr->ifile.eof()) {

                auto input2 = Utils::getInputString("Type Y to read from line or N to exit.");

                if (input2 == "Y") {
                    fcpa->getCommand();
                    cout << "-----LIST START-------" << endl;
                    auto list = fcpa->getCommandList();
                    for (auto i: list) {
                        std::cout << *i <<endl;
                    }
                    cout << "-----LIST END-------" << endl;
                } else if (input2 == "N")
                    break;
                else cout << "invalid command. Y or N only";
            }
            cout << "We left the file reader! Going back to main menu..." << endl;
        } else if (input1 == "leave") {
            break;
        } else {
            cout << "Command invalid. Try again.";
            continue;
        }

    }


}