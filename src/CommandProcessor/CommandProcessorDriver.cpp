#include <iostream>
#include <istream>
#include "CommandProcessor.h"

int main() {

    CommandProcessor * cp = new CommandProcessor();
    cp->getCommand();
    auto list = cp->getCommandList();

    for (auto i : list) {
        std::cout << *i;
    }
    return 0;
}