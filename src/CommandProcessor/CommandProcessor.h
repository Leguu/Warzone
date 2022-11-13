#ifndef WARZONE_SRC_COMMANDPROCESSOR_COMMANDPROCESSOR_H
#define WARZONE_SRC_COMMANDPROCESSOR_COMMANDPROCESSOR_H

#include "../Logging/LoggingObserver.h"
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

using std::cout;
using std::endl;
using std::ifstream;
using std::ostream;
using std::streampos;
using std::string;
using std::vector;

// Command class
class Command : public ILoggable, public Subject {
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

  std::string stringToLog() override;

private:
  string command;
  string arg;
  string effect;

  friend class CommandProcessor;
  friend void testLoggingObserver();
  friend class FileCommandProcessorAdapter;
};

// Command processor class

class CommandProcessor : public ILoggable, public Subject {
public:
  CommandProcessor();

  CommandProcessor(const CommandProcessor &);

  virtual ~CommandProcessor();

  CommandProcessor &operator=(const CommandProcessor &);

  friend ostream &operator<<(ostream &, const CommandProcessor &);

  bool validate(Command *);

  Command *getCommand();

  [[nodiscard]] Command *getCommand(const string &prompt,
									string command = string(),
									string arg = string());

  vector<Command *> getCommandList();

  std::string stringToLog() override;

  void saveCommand(Command *command);

private:
  virtual Command *readCommand();

  vector<Command *> commands;
  friend void testLoggingObserver();
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

#endif // WARZONE_SRC_COMMANDPROCESSOR_COMMANDPROCESSOR_H
