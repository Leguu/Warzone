#include "CommandProcessor.h"
#include <fstream>
#include <utility>
#include "../Utils/Utils.h"
#include "../GameEngine/GameEngine.h"
#include "../Logging/LogObserver.h"
#include <chrono>
#include <ctime>


/**
 * Command constructor
 */
Command::Command() {
    this->Attach(LogObserver::instance());
}

/**
 *  Command destructor
 */
Command::~Command() {
    this->Detach(LogObserver::instance());
}


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
    return cout << "Name of command is: " << c.command
                << ", Argument: " << c.arg << endl
                << "Effect of command is: " << c.effect << endl;

}

/**
 *
 * @return returns the commands whether it is valid or
 * not after it is read, processed, logged and saved
 */
Command *CommandProcessor::getCommand() {
    auto command = readCommand();
    validate(command);
    saveCommand(command);
    return command;

}

/**
 *
 * @return Checks the command whether it is
 * valid at the current state of the game engine
 */
Command *CommandProcessor::readCommand() {
    string line;
    while (true) {
        line = Utils::getInputString();

        auto tokens = Utils::tokenizer(line, ' ');


        if (Utils::isEqualLowercase(tokens[0], "loadmap") || Utils::isEqualLowercase(tokens[0], "addplayer")) {
            if (tokens.size() == 1) {
                cout << "This command is missing an argument." << endl;
                continue;
            }
            auto command = new Command();
            command->command = Utils::trim(tokens[0]);
            command->arg = Utils::trim(line.substr(tokens[0].length()));
            return command;

        } else if (Utils::isEqualLowercase(tokens[0], "validatemap") ||
                   Utils::isEqualLowercase(tokens[0], "gamestart") || Utils::isEqualLowercase(tokens[0], "replay") ||
                   Utils::isEqualLowercase(tokens[0], "quit")) {
            auto command = new Command();
            command->command = Utils::trim(line);
            return command;
        } else {
            cout << "You need to input something" << endl;
            continue;
        }
    }
}

/**
 *
 * @param command Just an average command object
 * @return checks whether this command is valid according
 * to its argument and returns a bool
 */
bool CommandProcessor::validate(Command *command) {

    auto engine = GameEngine::instance();

    string badString = "Invalid command!";

    if (engine != nullptr && command != nullptr) {

        GameEngine::GameState currState = engine->getState();
        string okString = "Valid command. Moving from state " + GameEngine::gameStates[currState];

        if (command->getCommand() == "loadmap" && !command->getArg().empty()) {
            if (currState == GameEngine::START || currState == GameEngine::MAP_LOADED) {
                command->saveEffect(okString);
                return true;
            }
        } else if (command->getCommand() == "validatemap") {
            if (currState == GameEngine::MAP_LOADED) {
                command->saveEffect(okString);
                return true;
            }
        } else if (command->getCommand() == "addplayer" && !command->getArg().empty()) {
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

/**
 * Default constructor for Command processor
 */
CommandProcessor::CommandProcessor() {
    this->Attach(LogObserver::instance());
}
/**
 * Copy constructor for Command processor
 */
CommandProcessor::CommandProcessor(const CommandProcessor &commandProcessor) {
    this->commands = commandProcessor.commands;
    this->Attach(LogObserver::instance());
}

/**
 * Destructor for Command processor
 */
CommandProcessor::~CommandProcessor() {
    this->Detach(LogObserver::instance());
}
/**
 * Assignment operator for Command processor
 * @param commandProcessor The command processor that we want to assign its values to another
 * @return Assigns the same values and pointers from one command processor to another
 */
CommandProcessor &CommandProcessor::operator=(const CommandProcessor &commandProcessor) = default;

ostream &operator<<(ostream &os, const CommandProcessor &commandProcessor) {
    os << "list of commands: \n";

    for (auto i: commandProcessor.commands) {
        os << i->getCommand() << endl;
    }
    return os;
}

/**
 * Command that gets pushed to a vector
 * @param command A command object whether valid or not
 */
void CommandProcessor::saveCommand(Command *command) {
    commands.push_back(command);
    this->Notify(this);

}

/**
 * Get method for command
 * @return A command is returned
 */
const string &Command::getCommand() const {
    return command;
}

/**
 * Get method for argument
 * @return
 */
const string &Command::getArg() const {
    return arg;
}

/**
 * Get method for effect
 * @return
 */
const string &Command::getEffect() const {
    return effect;
}

/**
 * Saves the effect into the command object
 * @param Effect A string that describes the command
 */
void Command::saveEffect(const string &Effect) {
    effect = Effect;
    this->Notify(this);
}

/**
 *
 * @param rhs
 * @return Checks if commands are the same
 */
bool Command::operator==(const string &rhs) const {
    return Utils::isEqualLowercase(command, rhs);
}
/**
 *
 * @param rhs
 * @return Checks if commands are the NOT the same
 */
bool Command::operator!=(const string &rhs) const {
    return !Utils::isEqualLowercase(command, rhs);
}

/**
 * Get command method
 * @param prompt the actual command from either a console or file
 * @return
 */
Command *CommandProcessor::getCommand(const string &prompt) {
    cout << prompt << endl;
    return getCommand();
}

/**
 * Logs commands from the command processor
 * @return
 */
std::string CommandProcessor::stringToLog() {
    std::ofstream file;
    file.open("../logs/gamelog.txt", std::ios_base::app);
    auto time = std::chrono::system_clock::now();
    std::time_t time_t = std::chrono::system_clock::to_time_t(time);
    file << std::ctime(&time_t);
    file << "New Command Added: " << this->commands[this->commands.size() - 1]->command << std::endl << std::endl;
    return "New Command Added: " + this->commands[this->commands.size() - 1]->command;
}

/**
 * Logs commands from the command object
 * @return
 */
std::string Command::stringToLog() {
    std::ofstream file;
    file.open("../logs/gamelog.txt", std::ios_base::app);
    auto time = std::chrono::system_clock::now();
    std::time_t time_t = std::chrono::system_clock::to_time_t(time);
    file << std::ctime(&time_t);
    file << "Effect Modified: " << this->effect << std::endl << std::endl;
    return "Effect Modified: " + this->effect;
}

/**
 * Get method for the command list vector
 * @return
 */
vector<Command *> CommandProcessor::getCommandList() {
    return commands;
}

/**
 * Default constructor for the adapter
 */
FileCommandProcessorAdapter::FileCommandProcessorAdapter() : CommandProcessor() {
    flr = new FileLineReader();
    this->Attach(LogObserver::instance());

}

/**
 * Destructor for the adapter
 */
FileCommandProcessorAdapter::~FileCommandProcessorAdapter() {
    delete flr;
    this->Detach(LogObserver::instance());
}

/**
 * Another constructor for the adapter
 * @param path A path to where the file is located to read commands
 */
FileCommandProcessorAdapter::FileCommandProcessorAdapter(string path) : CommandProcessor() {
    flr = new FileLineReader(std::move(path));
    this->Attach(LogObserver::instance());
}

/**
 * Copy constructor for the adapter
 * @param fcpa Another adapter to copy from
 */
FileCommandProcessorAdapter::FileCommandProcessorAdapter(const FileCommandProcessorAdapter &fcpa) : CommandProcessor(
        fcpa) {
    this->flr = fcpa.flr;
    this->Attach(LogObserver::instance());
}

/**
 * Assignemnt operator for the adapter
 * @param fcpa
 * @return
 */
FileCommandProcessorAdapter &FileCommandProcessorAdapter::operator=(const FileCommandProcessorAdapter &fcpa) {
    CommandProcessor::operator=(fcpa);
    this->flr = fcpa.flr;
    return *this;
}

/**
 * Insertion operator for the adapter
 * @param os
 * @param fcpa
 * @return
 */
ostream &operator<<(ostream &os, const FileCommandProcessorAdapter &fcpa) {
    os << "The path of the file command processor adaptor is: " << fcpa.flr->getPath() << endl;;
    return os;
}

/**
 * Line reader default constructor
 */
FileLineReader::FileLineReader() = default;

/**
 * Line reader default destructor
 */
FileLineReader::~FileLineReader() = default;

/**
 * Another Line reader constructor
 */
FileLineReader::FileLineReader(string newPath) {
    path = std::move(newPath);
}

/**
 * Line reader copy constructor
 * @param flr
 */
FileLineReader::FileLineReader(const FileLineReader &flr) {
    path = flr.path;
}
/**
 * Line reader Assignment operator
 * @param flr
 */
FileLineReader &FileLineReader::operator=(const FileLineReader &flr) {
    this->path = flr.path;
    return *this;
}

/**
 * Insertion operator for line reader
 * @param os
 * @param flr
 * @return
 */
ostream &operator<<(ostream &os, const FileLineReader &flr) {
    os << "The path of the file line reader is: " << flr.path << endl;
    return os;
}
/**
 * Get method for path
 * @return
 */
string FileLineReader::getPath() {
    return path;
}

/**
 * Set method for path
 * @param newPath
 */
void FileLineReader::setPath(string newPath) {
    path = std::move(newPath);
}

/**
 * Method to read get one line from a text file
 * @return
 */
string FileLineReader::readLineFromFile() {
    string line;
    if (!ifile) {
        throw runtime_error("File " + path + " could not be opened!");
    } else if (getline(ifile, line)) {
        return line;
    } else {
        ifile.close();
        return line;
    }


}
/**
 * Read commands from the file line by line and returns a command
 * @return
 */
Command *FileCommandProcessorAdapter::readCommand() {

    string currentLine = flr->readLineFromFile();

    auto tokens = Utils::tokenizer(currentLine, ' ');
    auto command = new Command();
    if (tokens[0] == "loadmap" || tokens[0] == "addplayer") {
        if (tokens.size() == 1) {
            cout << "This command is missing an argument. Moving on..." << endl;
        }
        command->command = Utils::trim(tokens[0]);
        command->arg = Utils::trim(currentLine.substr(tokens[0].length()));
        return command;
    } else if (tokens[0] == "validatemap" || tokens[0] == "gamestart" || tokens[0] == "replay" ||
               tokens[0] == "quit") {
        command->command = Utils::trim(currentLine);
        return command;
    } else {
        cout << "Some input must be here. Moving on." << endl;
        command->command = Utils::trim(tokens[0]);
        command->arg = Utils::trim(currentLine.substr(tokens[0].length()));
        return command;
    }
}







