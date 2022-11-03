#ifndef WARZONE_SRC_COMMANDPROCESSOR_COMMANDPROCESSOR_H
#define WARZONE_SRC_COMMANDPROCESSOR_COMMANDPROCESSOR_H

#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <fstream>

using std::string;
using std::vector;
using std::cout;
using std::endl;
using std::ifstream;
using std::streampos;
using std::ostream;

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

    friend class FileCommandProcessorAdapter;
};

// Command processor class

class CommandProcessor {
public:

    CommandProcessor();

    CommandProcessor(const CommandProcessor &);

    virtual ~CommandProcessor();

    CommandProcessor &operator=(const CommandProcessor &);

    friend ostream &operator<<(ostream &, const CommandProcessor &);

    bool validate(Command *);

    Command *getCommand();

    [[nodiscard]] Command *getCommand(const string &prompt);

    vector<Command *> getCommandList();

private:

    virtual Command *readCommand();

    void saveCommand(Command *command);

    vector<Command *> commands;

    friend class FileCommandProcessorAdapter;
};

// File line reader

class FileLineReader {
public:

    FileLineReader();

    explicit FileLineReader(string);

    FileLineReader(const FileLineReader &);

    ~FileLineReader();

    FileLineReader &operator=(const FileLineReader &);

    friend ostream &operator<<(ostream &os, const FileLineReader &);

    string getPath();

    void setPath(string);

    string readLineFromFile();

    ifstream ifile;
private:

    string path;
};


// File command processor class

class FileCommandProcessorAdapter : public CommandProcessor {
public:

    FileCommandProcessorAdapter();

    explicit FileCommandProcessorAdapter(string path);

    FileCommandProcessorAdapter(const FileCommandProcessorAdapter &);

    ~FileCommandProcessorAdapter() override;

    FileCommandProcessorAdapter &operator=(const FileCommandProcessorAdapter &);

    friend ostream &operator<<(ostream &, const FileCommandProcessorAdapter &);

    FileLineReader *flr = nullptr;

private:
    Command *readCommand() override;

};

#endif //WARZONE_SRC_COMMANDPROCESSOR_COMMANDPROCESSOR_H
