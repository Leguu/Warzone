//
// Created by Legu on 2022-10-17.
//

#include "CommandProcessor.h"
#include <fstream>
#include "../Utils/Utils.h"

Command *CommandProcessor::getCommand() {
    auto command = readCommand();

    saveCommand(command);

    return command;
}

Command *CommandProcessor::readCommand() {
    string line;
    while (true) {
        line = Utils::getInputString();

        auto tokens = Utils::tokenizer(line, ' ');

        if (tokens.empty()) {
            cout << "You need to input something" << endl;
            continue;
        }

        if (tokens[0] == "loadmap" || tokens[0] == "addplayer") {
            if (tokens.size() == 1) {
                cout << "This command is missing an argument." << endl;
            }
            auto command = new Command();
            command->command = Utils::trim(tokens[0]);
            command->arg = Utils::trim(line.substr(tokens[0].length()));
            return command;
        } else {
            auto command = new Command();

            command->command = Utils::trim(line);

            return command;
        }
    }
}

void CommandProcessor::saveCommand(Command *command) {
    commands.push_back(command);
    this->Notify(this);
}

const string &Command::getCommand() const {
    return command;
}

const string &Command::getArg() const {
    return arg;
}

const string &Command::getEffect() const {
    return effect;
}

void Command::setEffect(const string &Effect) {
    effect = Effect;
    this->Notify(this);

}

bool Command::operator==(const string &rhs) const {
    return Utils::isEqualLowercase(command, rhs);
}

bool Command::operator!=(const string &rhs) const {
    return !Utils::isEqualLowercase(command, rhs);
}

Command *CommandProcessor::getCommand(const string &prompt) {
    cout << prompt << endl;
    return getCommand();
}

std::string CommandProcessor::stringToLog() {
    std::ofstream file;
    file.open("../logs/gamelog.txt", std::ios_base::app);
    file << "New Command Added: " << this->commands[this->commands.size() - 1]->command << std::endl << std::endl;
    return "New Command Added: " + this->commands[this->commands.size() - 1]->command;
}

std::string Command::stringToLog() {
    std::ofstream file;
    file.open("../logs/gamelog.txt", std::ios_base::app);
    file << "Effect Modified: " << this->effect << std::endl << std::endl;
    return "Effect Modified: " + this->effect;
}
