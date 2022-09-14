//
// Created by Legu on 2022-09-14.
//

#include <iostream>
#include "Utils.h"

const auto inputPrompt = ">";

std::string Utils::getInputString(const std::string &prompt) {
    std::cout << prompt << std::endl;
    std::cout << inputPrompt << std::flush;
    std::string input;
    std::cin >> input;
    return input;
}

int Utils::getInputInt(const std::string &prompt) {
    std::cout << prompt << std::endl;
    std::string input;
    int value;
    while (true) {
        std::cout << inputPrompt << std::flush;
        std::cin >> input;
        try {
            value = std::stoi(input);
            break;
        } catch (std::invalid_argument &e) {
            std::cout << "Your input has to be a number!" << std::endl;
            continue;
        }
    }
    return value;
}
