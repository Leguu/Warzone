#ifndef WARZONE_SRC_COMMANDPROCESSOR_COMMANDPROCESSOR_H
#define WARZONE_SRC_COMMANDPROCESSOR_COMMANDPROCESSOR_H

#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include "../Logging/LoggingObserver.h"

using std::string;
using std::vector;
using std::cout;
using std::endl;

class CommandProcessor;

class Command : public ILoggable, public Subject {
public:
    [[nodiscard]] const string &getCommand() const;

    [[nodiscard]] const string &getArg() const;

    [[nodiscard]] const string &getEffect() const;

    bool operator!=(const string &rhs) const;

    bool operator==(const string &rhs) const;

    void setEffect(const string &Effect);

    std::string stringToLog() override;


private:
    string command;
    string arg;
    string effect;

    friend class CommandProcessor;
};

class CommandProcessor : public ILoggable, public Subject {
    Command *getCommand();

public:
    [[nodiscard]] Command *getCommand(const string &prompt);

    std::string stringToLog() override;

private:
    static Command *readCommand();

    void saveCommand(Command *command);

    vector<Command *> commands;
};

#endif //WARZONE_SRC_COMMANDPROCESSOR_COMMANDPROCESSOR_H
