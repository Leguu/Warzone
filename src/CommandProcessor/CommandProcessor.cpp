#include "CommandProcessor.h"

#include <utility>
#include "../Utils/Utils.h"


/**
 * Command constructor
 */
Command::Command() = default;

/**
 *  Command destructor
 */
Command::~Command() = default;


/**
 * Copy constructor for command
 * @param command name of the command
 */
Command::Command(const Command &command) {
    this->command = command.command;
    this->arg = command.arg;
    this->effect = command.effect;
}

/**
 * Overload for the = operator on command
 * @param c command we are assigning to
 * @return Copy constructor used to create new command
 */
Command &Command::operator=(const Command &c) {
    this->command = c.command;
    this->effect = c.effect;
    return *this;
}

/**
 * Insertion operator for command class
 * @param os stream operator
 * @param c command class
 * @return information about the command
 */
ostream &operator<<(ostream &os, const Command &c) {
    return cout << "the name of the command is " << c.command << endl
                << ", and the effect of the command is " << c.effect << endl
                << ", it's argument is " << c.arg << endl;
}

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

        if (tokens[0] == "loadmap" || tokens[0] == "validatemap" || tokens[0] == "addplayer" ||
            tokens[0] == "gamestart" || tokens[0] == "replay" || tokens[0] == "quit") {
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

CommandProcessor::CommandProcessor() = default;

CommandProcessor::CommandProcessor(const CommandProcessor &commandProcessor) {
    this->commands = commandProcessor.commands;
}

CommandProcessor::~CommandProcessor() = default;

CommandProcessor &CommandProcessor::operator=(const CommandProcessor &commandProcessor) = default;

ostream &operator<<(ostream &os, const CommandProcessor &commandProcessor) {
    // TODO format this
     os << "list of commands: \n" ;

     for (auto i: commandProcessor.commands) {
         os << i->getCommand() << endl;
     }
     return os;
}

void CommandProcessor::saveCommand(Command *command) {
    commands.push_back(command);
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

void Command::saveEffect(const string &Effect) {
    effect = Effect;
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







