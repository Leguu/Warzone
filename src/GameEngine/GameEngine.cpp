#include <iostream>

#include <fstream>

#include <random>

#include <chrono>

#include <ctime>

#include "GameEngine.h"

#include "../Logging/LogObserver.h"

GameEngine *GameEngine::_instance = nullptr;

const string GameEngine::helpText =
        "Available commands:\n"
        "help - display this text\n"
        "play <card> - play a card from your hand\n"
        "list hand - list what cards are in your hand\n"
        "list orders - list what orders you've submitted so far\n"
        "advance - create an advance order\n"
        "deploy - create an deploy order\n"
        "map - display the map\n"
        "territory <name> - display some information about a territory\n"
        "done - indicate that you're finished with your turn";

const string GameEngine::wrongStateTransitionMessage = "Wrong state";

/**
 * GameEngine constructor used to create a singleton game engine used to manage
 * the game
 * @param mp the map
 */
GameEngine::GameEngine(const std::string &mp) {
  map = MapLoader::importMap(mp);
  _instance = this;
  this->Attach(LogObserver::instance());
}

/// Execute all the orders
/// \return whether win condition has been reached

void GameEngine::executeOrdersPhase() {
  cout << endl
       << "Executing orders..." << endl;

  while (true) {
    for (auto player: players) {
      auto allDoneExecuting = std::all_of(players.begin(), players.end(), [](Player *p) {
        return p->orders->getOrdersSize() == 0;
      });

      if (allDoneExecuting) {
        goto outerloop;
      }

      auto order = player->orders->pop();

      if (order == nullptr) {
        continue;
      }

      try {
        cout << *order << endl;
        order->execute();
      } catch (InvalidOrderException &e) {
        cout << order->name << " order issued by " + order->issuer->name
             << " was invalid: " << e.what() << endl;
      }
    }
  }
outerloop:

  cout << "... All orders have been executed" << endl
       << endl;
  cout << "Next round has started" << endl
       << endl;
}

/**
 * Game loop to run the game
 */

void GameEngine::mainGameLoop() {
  while (true) {
    reinforcementPhase();

    issueOrdersPhase();

    executeOrdersPhase();

    turnsGone += 1;

    if (players.size() == 1) {
      cout << "It has been " << turnsGone << " turns since the game has started." << endl;
      break;
    }

    if (turnsGone >= maxTurns) {
      cout << "Max turns have been reached! The game is over." << endl;
      break;
    }

    for (auto player: players) {
      player->cardAwarded = false;
      player->strategy->resetTurn();
      player->reinforcementsAfterDeploy = 0;
      player->cannotAttack.clear();
    }
  }
}

/**
 * Find a player by name
 * @param name The name of the player
 * @return The player object
 */
Player *GameEngine::findPlayerByName(const std::string &name) {
  for (auto *player: this->players) {
    if (player->name == name) {
      return player;
    }
  }
  return nullptr;
}

/**
 * Empty constructor to create the game engine singleton variable
 */
GameEngine::GameEngine() {
  _instance = this;
  this->Attach(LogObserver::instance());
}

/**
 * Issue the orders
 */
void GameEngine::issueOrdersPhase() {
  while (true) {
    for (auto player: players) {
      auto allPlayersDone = std::all_of(players.begin(), players.end(), [](auto item) {
        return item->strategy->isDoneIssuing();
      });

      if (allPlayersDone) {
        return;
      }

      if (player->strategy->isDoneIssuing() ||
          std::find(players.begin(), players.end(), player) == players.end()) {
        continue;
      }

      cout << player->name << " is issuing an order: " << endl;
      player->issueOrder();
    }
  }
}

/**
 * Get an instance of the game engine object
 * @return instance of the game engine object
 */
GameEngine *GameEngine::instance() {
  if (!_instance)
    throw runtime_error("GameEngine instance not yet initialised. Did you "
                        "forget to create a GameEngine first?");
  return _instance;
}

/**
 * Assign the reinforcements to all territories
 */
void GameEngine::reinforcementPhase() const {
  for (auto p: players) {
    auto reinforcementsToAssign = 0;

    reinforcementsToAssign += p->ownedTerritories.size();

    for (auto c: map->getContinents()) {
      if (c->owner() == p) {
        reinforcementsToAssign += c->getBonus();
      }
    }

    reinforcementsToAssign = std::min(3, reinforcementsToAssign);

    p->reinforcements += reinforcementsToAssign;
    p->reinforcementsAfterDeploy = p->reinforcements;
  }
}

bool GameEngine::startupPhase() {
  const string commands =
          "Available commands:\n"
          "LoadMap <Map Name> - Load a map\n"
          "ValidateMap - Validate the map has been loaded successfully\n"
          "AddPlayer <Name> - Add a player\n"
          "GameStart - Start the game\n"
          "Replay - Restart the game\n"
          "Quit - Quit the game";

  Command *input;
  transition(GameState::START);

  while (true) {
    input = commandProcessor->getCommand("Input your command, write \"help\" for help");

    if (input == nullptr) {
      continue;
    }
    std::string inputText = input->getCommand();

    if (Utils::isEqualLowercase(inputText, "tournament")) {
      tournamentMode(input);
      return true;
    }

    if (Utils::isEqualLowercase(inputText.substr(0, inputText.find(' ')),
                                "loadmap")) {
      loadMap("../assets/" + input->getArg() + ".map");
    } else if (*input == "help") {
      cout << commands << endl;
    } else if (Utils::isEqualLowercase(inputText, "validatemap")) {
      validateMap();
    } else if (Utils::isEqualLowercase(inputText.substr(0, inputText.find(' ')),
                                       "addplayer")) {
      addPlayer(input->getArg());
    } else if (Utils::isEqualLowercase(inputText, "gamestart")) {
      if (state != GameState::PLAYERS_ADDED) {
        cout << wrongStateTransitionMessage << endl;
        continue;
      }

      if (players.size() < 2) {
        cout << "You can't have"
                " a game of less than 2 players, unless you're "
                "schizophrenic"
             << endl;
        continue;
      }

      assignCountries();

      for (auto p: players) {
        p->hand->draw();
        p->hand->draw();
      }

      mainGameLoop();

      cout << "Game is over! Player " << players[0]->name << " has won!" << endl;

      transition(GameState::WIN);
    } else if (Utils::isEqualLowercase(input->getCommand(), "quit")) {
      if (state != GameState::WIN) {
        cout << wrongStateTransitionMessage << endl;
        continue;
      }
      return true;
    } else if (Utils::isEqualLowercase(input->getCommand(), "replay")) {
      if (state != GameState::WIN) {
        cout << wrongStateTransitionMessage << endl;
        continue;
      }
      return true;
    } else {
      cout << "Type the right command" << endl;
    }
  }
}

/**
 * Load the map like the teacher wants
 * @param input the map name
 */
void GameEngine::loadMap(const string &input) {
  if (state != MAP_LOADED && state != START) {
    cout << wrongStateTransitionMessage << endl;
    return;
  }

  if (input.empty()) {
    cout << "You need to input the map name!" << endl;
    return;
  }

  try {
    map = MapLoader::importMap(input);
  } catch (runtime_error &e) {
    cout << "Error with that map file: " << e.what() << endl;
    return;
  }

  transition(GameState::MAP_LOADED);
}

/**
 * Validate the map like the teacher wants
 */

void GameEngine::validateMap() {
  if (state != MAP_LOADED) {
    cout << wrongStateTransitionMessage << endl;
    return;
  }
  try {
    map->validate();
  } catch (runtime_error &e) {
    cout << "This map is not valid: " << e.what() << endl;
  }
  transition(GameState::MAP_VALIDATED);
}

/**
 * Add player like teacher wants
 * @param playerName The player name
 */
void GameEngine::addPlayer(const string &playerName) {
  if (state != MAP_VALIDATED && state != PLAYERS_ADDED) {
    cout << wrongStateTransitionMessage << endl;
    return;
  }

  if (players.size() >= 6) {
    cout << "Can't add more than 6 players!" << endl;
    return;
  }

  if (playerName.empty()) {
    cout << "Can't add a player named nothing baka" << endl;
    return;
  }
  players.push_back(new Player(playerName));
  transition(GameState::PLAYERS_ADDED);
}

/**
 * Assign the countries like the teacher wants
 */
void GameEngine::assignCountries() {
  auto rng = std::default_random_engine();
  rng.seed(std::chrono::system_clock::now().time_since_epoch().count());
  std::shuffle(players.begin(), players.end(), rng);

  auto i = 0;
  for (auto t: map->getAllTerritories()) {
    t->owner = nullptr;
    t->setArmies(0);
    t->setOwner(players[i]);
    i = (i + 1) % players.size();
  }
}

/**
 * Destructor for the game engine
 */
GameEngine::~GameEngine() {
  _instance = nullptr;
  this->Detach(LogObserver::instance());
  for (auto p: players) {
    delete p;
  }
  delete deck;
  delete map;
}

std::string GameEngine::stringToLog() {
  std::ofstream file;
  file.open("../logs/gamelog.txt", std::ios_base::app);
  auto time = std::chrono::system_clock::now();
  std::time_t time_t = std::chrono::system_clock::to_time_t(time);
  file << std::ctime(&time_t);
  file << "Game State Modified: " << this->gameStates[this->state] << std::endl
       << std::endl;
  return "Game State Modified: " + this->gameStates[this->state];
}

GameEngine::GameState GameEngine::getState() { return state; }

void GameEngine::transition(GameEngine::GameState newState) {
  this->state = newState;
  this->Notify(this);
}

void GameEngine::tournamentMode(Command *command) {
  auto args = Utils::tokenizer(command->getArg(), ' ');

  vector<Map *> mapsToPlayOn = {};
  vector<Player *> playersToPlay = {};
  string mode;
  auto playerNameIncrement = 0;
  for (const auto &argument: args) {
    auto arg = Utils::trim(Utils::toLowercase(argument));
    if (Utils::isEqualLowercase(arg, "-m") ||
        Utils::isEqualLowercase(arg, "-p") ||
        Utils::isEqualLowercase(arg, "-g") ||
        Utils::isEqualLowercase(arg, "-d")) {
      mode = arg;
      continue;
    }

    if (mode.empty()) {
      cout << "Invalid command" << endl;
      return;
    }

    if (mode == "-m") {
      auto mapToPlayOn = MapLoader::importMap("../assets/" + arg + ".map");
      mapToPlayOn->name = arg;
      mapsToPlayOn.push_back(mapToPlayOn);
    } else if (mode == "-p") {
      auto player = new Player(arg + std::to_string(playerNameIncrement));
      playerNameIncrement += 1;

      if (Utils::isEqualLowercase(arg, "aggressive")) {
        player->strategy = new AggressivePlayerStrategy(player);
      } else if (Utils::isEqualLowercase(arg, "cheater")) {
        player->strategy = new CheaterStrategy(player);
      }else if (Utils::isEqualLowercase(arg, "benevolent")) {
        player->strategy = new BenevolentPlayer(player);
      }else if (Utils::isEqualLowercase(arg, "human")) {
        player->strategy = new HumanStrategy(player);
      } else if (Utils::isEqualLowercase(arg, "neutral")) {
        player->strategy = new NeutralStrategy(player);
      } else if (Utils::isEqualLowercase(arg, "default")) {
        player->strategy = new DefaultPlayerStrategy(player);
      } else {
        player->strategy = new HumanStrategy(player);
      }

      playersToPlay.push_back(player);
    } else if (mode == "-g") {
      gamesToRun = std::stoi(arg);
    } else if (mode == "-d") {
      maxTurns = std::stoi(arg);
    }
  }

  if (playersToPlay.size() < 2) {
    cout << "Can't have a game with fewer than 2 players. Don't forget to use -P to initialise players" << endl;
    return;
  }

  if (mapsToPlayOn.empty()) {
    cout << "Missing -M map" << endl;
    return;
  }

  auto logFile = std::fopen("../tournament.txt", "w");

  if (!logFile) {
    cout << "Log file couldn't be opened. Aborting..." << endl;
    return;
  }

  std::fprintf(logFile, "Running this command: `%s %s`\n\n", command->getCommand().c_str(), command->getArg().c_str());

  std::fprintf(logFile, "%s\n\n", ("Playing " + std::to_string(gamesToRun) + " with " + std::to_string(maxTurns) + " maximum turns per game").c_str());

  std::fprintf(logFile, "%10s", "Map");

  for (int i = 1; i <= gamesToRun; i += 1) {
    std::fprintf(logFile, " | %16s", ("Game " + std::to_string(i)).c_str());
  }

  std::fprintf(logFile, "\n");

  for (auto mapToPlayOn: mapsToPlayOn) {
    map = mapToPlayOn;
    turnsGone = 0;

    std::fprintf(logFile, "%10s", map->name.c_str());

    for (int i = 0; i < gamesToRun; i += 1) {
      players = playersToPlay;

      for (auto player: players) {
        player->ownedTerritories.clear();
        player->reinforcements = 50;
        player->orders->clear();
        player->hand->removeAll();
        player->hand->draw();
        player->hand->draw();
      }

      assignCountries();

      mainGameLoop();

      if (this->turnsGone < maxTurns)
        std::fprintf(logFile, " | %16s", players[0]->name.c_str());
      else
        std::fprintf(logFile, " | %16s", "Draw");
    }

    std::fprintf(logFile, "\n");

    delete mapToPlayOn;
    map = nullptr;
  }

  std::fclose(logFile);
}
