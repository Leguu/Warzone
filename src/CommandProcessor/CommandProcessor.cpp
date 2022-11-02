#include "CommandProcessor.h"

#include <utility>
#include "../Utils/Utils.h"
#include "../GameEngine/GameEngine.h"


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
    this->arg = c.arg;
    return *this;
}

/**
 * Insertion operator for command class
 * @param os stream operator
 * @param c command class
 * @return information about the command
 */
ostream &operator<<(ostream &os, const Command &c) {
    return cout << "The name of the command is: " << c.command << endl
                << "The effect of the command is: " << c.effect << endl
                << "It's argument: " << c.arg << endl;
}

Command *CommandProcessor::getCommand() {
    auto command = readCommand();
    validate(command);
    saveCommand(command);

    return command;
}

Command *CommandProcessor::readCommand() {
    string line;
    while (true) {
        line = Utils::getInputString();

        auto tokens = Utils::tokenizer(line, ' ');

        if (tokens[0] == "loadmap" || tokens[0] == "addplayer") {
            if (tokens.size() == 1) {
                cout << "This command is missing an argument." << endl;
                continue;
            }
            auto command = new Command();
            command->command = Utils::trim(tokens[0]);
            command->arg = Utils::trim(line.substr(tokens[0].length()));
            return command;
        } else if (tokens[0] == "validatemap" || tokens[0] == "gamestart" || tokens[0] == "replay" ||
                   tokens[0] == "quit") {
            auto command = new Command();
            command->command = Utils::trim(line);
            return command;
        } else {
            cout << "You need to input something" << endl;
            continue;
        }
    }
}

bool CommandProcessor::validate(Command *command) {

    auto engine = GameEngine::instance();

    string badString = "Invalid command!";

    if (engine != nullptr && command != nullptr) {

        GameEngine::GameState currState = engine->getState();
        string okString = "Valid command. Moving from state " + GameEngine::toString(currState);

        if (command->getCommand() == "loadmap") {
            if (currState == GameEngine::START || currState == GameEngine::MAP_LOADED) {
                command->saveEffect(okString);
                return true;
            }
        } else if (command->getCommand() == "validatemap") {
            if (currState == GameEngine::MAP_LOADED) {
                command->saveEffect(okString);
                return true;
            }
        } else if (command->getCommand() == "addplayer") {
            if (currState == GameEngine::MAP_VALIDATED || currState == GameEngine::PLAYERS_ADDED) {
                command->saveEffect(okString);
                return true;
            }
        } else if (command->getCommand() == "gamestart") {
            if (currState == GameEngine::PLAYERS_ADDED) {
                command->saveEffect(okString);
                return true;
            }
        } else if (command->getCommand() == "replay" || command->getCommand() == "quit") {
            if (currState == GameEngine::WIN) {
                command->saveEffect(okString);
                return true;
            }
        }
    }
    command->saveEffect(badString);
    return false;
}


CommandProcessor::CommandProcessor() = default;

CommandProcessor::CommandProcessor(const CommandProcessor &commandProcessor) {
    this->commands = commandProcessor.commands;
}

CommandProcessor::~CommandProcessor() = default;

CommandProcessor &CommandProcessor::operator=(const CommandProcessor &commandProcessor) = default;

ostream &operator<<(ostream &os, const CommandProcessor &commandProcessor) {
    // TODO format this
    os << "list of commands: \n";

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

vector<Command *> CommandProcessor::getCommandList() {
    return commands;
}







