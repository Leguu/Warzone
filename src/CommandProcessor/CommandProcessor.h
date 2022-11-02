#ifndef WARZONE_SRC_COMMANDPROCESSOR_COMMANDPROCESSOR_H
#define WARZONE_SRC_COMMANDPROCESSOR_COMMANDPROCESSOR_H

#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <fstream>
#include "../GameEngine/GameEngine.h"

using std::string;
using std::vector;
using std::cout;
using std::endl;
using std::ifstream;
using std::streampos;
using std::ostream;

class CommandProcessor;

class GameEngine;

// Command class
class Command {
public:
    Command();

    Command(const Command &);

    virtual ~Command();

    Command &operator=(const Command &);

    friend ostream &operator<<(ostream &, const Command &);

    [[nodiscard]] const string &getCommand() const;

    [[nodiscard]] const string &getArg() const;

    [[nodiscard]] const string &getEffect() const;

    bool operator!=(const string &rhs) const;

    bool operator==(const string &rhs) const;

    void saveEffect(const string &Effect);

private:
    string command;
    string arg;
    string effect;

    friend class CommandProcessor;
};

// Command processor class

class CommandProcessor {
public:

    CommandProcessor();

    CommandProcessor(const CommandProcessor &);

    virtual ~CommandProcessor();

    CommandProcessor &operator=(const CommandProcessor &);

    friend ostream &operator<<(ostream &, const CommandProcessor &);

    bool validate(GameEngine* , Command* );

    Command *getCommand();

    [[nodiscard]] Command *getCommand(const string &prompt);

private:

    static Command *readCommand();

    void saveCommand(Command *command);

    vector<Command *> commands;

    friend class FileCommandProcessorAdapter;
};

// File line reader

class FileLineReader  {
    // TODO implement
};


// File command processor class

class FileCommandProcessorAdapter : public CommandProcessor {
    // TODO implement
};

#endif //WARZONE_SRC_COMMANDPROCESSOR_COMMANDPROCESSOR_H
