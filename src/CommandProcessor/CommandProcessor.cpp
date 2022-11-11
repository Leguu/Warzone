#include "CommandProcessor.h"
#include "../GameEngine/GameEngine.h"
#include "../Logging/LogObserver.h"
#include "../Utils/Utils.h"
#include <chrono>
#include <ctime>
#include <fstream>
#include <utility>

/**
 * Command constructor
 */
Command::Command() { this->Attach(LogObserver::instance()); }

/**
 *  Command destructor
 */
Command::~Command() { this->Detach(LogObserver::instance()); }

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
  return cout << "Name of command is: " << c.command << ", Argument: " << c.arg
			  << endl
			  << "Effect of command is: " << c.effect << endl;
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

	if (Utils::isEqualLowercase(tokens[0], "loadmap") ||
		Utils::isEqualLowercase(tokens[0], "addplayer")) {
	  if (tokens.size() == 1) {
		cout << "This command is missing an argument." << endl;
		continue;
	  }
	  auto command = new Command();
	  command->command = Utils::trim(tokens[0]);
	  command->arg = Utils::trim(line.substr(tokens[0].length()));
	  return command;

	} else if (Utils::isEqualLowercase(tokens[0], "validatemap") ||
		Utils::isEqualLowercase(tokens[0], "gamestart") ||
		Utils::isEqualLowercase(tokens[0], "replay") ||
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

bool CommandProcessor::validate(Command *command) {

  auto engine = GameEngine::instance();

  string badString = "Invalid command!";

  if (engine != nullptr && command != nullptr) {

	GameEngine::GameState currState = engine->getState();
	string okString =
		"Valid command. Moving from state " + GameEngine::gameStates[currState];

	if (command->getCommand() == "loadmap" && !command->getArg().empty()) {
	  if (currState == GameEngine::START ||
		  currState == GameEngine::MAP_LOADED) {
		command->saveEffect(okString);
		return true;
	  }
	} else if (command->getCommand() == "validatemap") {
	  if (currState == GameEngine::MAP_LOADED) {
		command->saveEffect(okString);
		return true;
	  }
	} else if (command->getCommand() == "addplayer" &&
		!command->getArg().empty()) {
	  if (currState == GameEngine::MAP_VALIDATED ||
		  currState == GameEngine::PLAYERS_ADDED) {
		command->saveEffect(okString);
		return true;
	  }
	} else if (command->getCommand() == "gamestart") {
	  if (currState == GameEngine::PLAYERS_ADDED) {
		command->saveEffect(okString);
		return true;
	  }
	} else if (command->getCommand() == "replay" ||
		command->getCommand() == "quit") {
	  if (currState == GameEngine::WIN) {
		command->saveEffect(okString);
		return true;
	  }
	}
  }
  command->saveEffect(badString);
  return false;
}

CommandProcessor::CommandProcessor() { this->Attach(LogObserver::instance()); }

CommandProcessor::CommandProcessor(const CommandProcessor &commandProcessor) {
  this->commands = commandProcessor.commands;
  this->Attach(LogObserver::instance());
}

CommandProcessor::~CommandProcessor() { this->Detach(LogObserver::instance()); }

CommandProcessor &
CommandProcessor::operator=(const CommandProcessor &commandProcessor) = default;

ostream &operator<<(ostream &os, const CommandProcessor &commandProcessor) {
  // TODO format this
  os << "list of commands: \n";

  for (auto i : commandProcessor.commands) {
	os << i->getCommand() << endl;
  }
  return os;
}

void CommandProcessor::saveCommand(Command *command) {
  commands.push_back(command);
  this->Notify(this);
}

const string &Command::getCommand() const { return command; }

const string &Command::getArg() const { return arg; }

const string &Command::getEffect() const { return effect; }

void Command::saveEffect(const string &Effect) {
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
  auto time = std::chrono::system_clock::now();
  std::time_t time_t = std::chrono::system_clock::to_time_t(time);
  file << std::ctime(&time_t);
  file << "New Command Added: "
	   << this->commands[this->commands.size() - 1]->command << std::endl
	   << std::endl;
  return "New Command Added: " +
	  this->commands[this->commands.size() - 1]->command;
}

std::string Command::stringToLog() {
  std::ofstream file;
  file.open("../logs/gamelog.txt", std::ios_base::app);
  auto time = std::chrono::system_clock::now();
  std::time_t time_t = std::chrono::system_clock::to_time_t(time);
  file << std::ctime(&time_t);
  file << "Effect Modified: " << this->effect << std::endl << std::endl;
  return "Effect Modified: " + this->effect;
}

vector<Command *> CommandProcessor::getCommandList() { return commands; }

FileCommandProcessorAdapter::FileCommandProcessorAdapter()
	: CommandProcessor() {
  flr = new FileLineReader();
  this->Attach(LogObserver::instance());
}

FileCommandProcessorAdapter::~FileCommandProcessorAdapter() {
  delete flr;
  this->Detach(LogObserver::instance());
}

FileCommandProcessorAdapter::FileCommandProcessorAdapter(string path)
	: CommandProcessor() {
  flr = new FileLineReader(std::move(path));
  this->Attach(LogObserver::instance());
}

FileCommandProcessorAdapter::FileCommandProcessorAdapter(
	const FileCommandProcessorAdapter &fcpa)
	: CommandProcessor(fcpa) {
  this->flr = fcpa.flr;
  this->Attach(LogObserver::instance());
}

FileCommandProcessorAdapter &FileCommandProcessorAdapter::operator=(
	const FileCommandProcessorAdapter &fcpa) {
  CommandProcessor::operator=(fcpa);
  this->flr = fcpa.flr;
  return *this;
}

ostream &operator<<(ostream &os, const FileCommandProcessorAdapter &fcpa) {
  os << "The path of the file command processor adaptor is: "
	 << fcpa.flr->getPath() << endl;;
  return os;
}

FileLineReader::FileLineReader() = default;

FileLineReader::~FileLineReader() = default;

FileLineReader::FileLineReader(string newPath) { path = std::move(newPath); }

FileLineReader::FileLineReader(const FileLineReader &flr) { path = flr.path; }

FileLineReader &FileLineReader::operator=(const FileLineReader &flr) {
  this->path = flr.path;
  return *this;
}

ostream &operator<<(ostream &os, const FileLineReader &flr) {
  os << "The path of the file line reader is: " << flr.path << endl;
  return os;
}

string FileLineReader::getPath() { return path; }

void FileLineReader::setPath(string newPath) { path = std::move(newPath); }

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
  } else if (tokens[0] == "validatemap" || tokens[0] == "gamestart" ||
	  tokens[0] == "replay" || tokens[0] == "quit") {
	command->command = Utils::trim(currentLine);
	return command;
  } else {
	cout << "Some input must be here. Moving on." << endl;
	command->command = Utils::trim(tokens[0]);
	command->arg = Utils::trim(currentLine.substr(tokens[0].length()));
	return command;
  }
}
