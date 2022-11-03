#include <iostream>
#include <random>
#include "GameEngine.h"

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
 * GameEngine constructor used to create a singleton game engine used to manage the game
 * @param mp the map
 */
GameEngine::GameEngine(const std::string &mp) {
  map = MapLoader::importMap(mp);
  _instance = this;
}

/// Execute all the orders
/// \return whether win condition has been reached
bool GameEngine::executeOrdersPhase() {
  cout << endl << "Executing orders..." << endl;

  for (auto &player : this->players) {
    while (auto order = player->orders->pop()) {
      try {
        order->execute();
        cout << *order << endl;
        if (map->allContinentsOwned()) {
          cout << "Game is over!" << endl;
          return true;
        }
      } catch (InvalidOrderException &e) {
        cout << order->name << " order issued by " + order->issuer->name << " was invalid: " << e.what()
             << endl;
      }
    }
  }
  cout << "... All orders have been executed" << endl << endl;
  return false;
}

/**
 * Game loop to run the game
 */
void GameEngine::mainGameLoop() {
  cout << "Welcome to Warzone! Type in \"help\" at any time to have a list of commands" << endl;
  while (true) {
    reinforcementPhase();

    issueOrdersPhase();

    auto gameOver = executeOrdersPhase();

    // If the game is over
    if (gameOver) {
      break;
    }
  }

  cout << "Game is over!" << endl;
}

/**
 * Find a player by name
 * @param name The name of the player
 * @return The player object
 */
Player *GameEngine::findPlayerByName(const std::string &name) {
  for (auto *player : this->players) {
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
}

/**
 * Issue the orders
 */
void GameEngine::issueOrdersPhase() {
  for (auto player : players) {
    player->issueOrder();
  }
}

/**
 * Get an instance of the game engine object
 * @return instance of the game engine object
 */
GameEngine *GameEngine::instance() {
  if (!_instance)
    throw runtime_error("GameEngine instance not yet initialised. Did you forget to create a GameEngine first?");
  return _instance;
}

/**
 * Assign the reinforcements to all territories
 */
void GameEngine::reinforcementPhase() const {
  for (auto p : players) {
    auto reinforcementsToAssign = 0;

    reinforcementsToAssign += p->ownedTerritories.size();

    for (auto c : map->getContinents()) {
      if (c->owner() == p) {
        reinforcementsToAssign += c->getBonus();
      }
    }

    reinforcementsToAssign = std::min(3, reinforcementsToAssign);

    p->reinforcements += reinforcementsToAssign;
  }
}

void GameEngine::startupPhase() {
  state = START;
  const string commands =
      "Available commands:\n"
      "LoadMap <Map Name> - Load a map\n"
      "ValidateMap - Validate the map has been loaded successfully\n"
      "AddPlayer <Name> - Add a player\n"
      "GameStart - Start the game\n"
      "Replay - Restart the game\n"
      "Quit - Quit the game";

  Command *input;

  while (true) {
    input = commandProcessor->getCommand("Input your command, write \"help\" for help");

    if (input == nullptr) {
      continue;
    }

    if (*input == "loadmap") {
      loadMap("../assets/" + input->getArg() + ".map");
    } else if (*input == "help") {
      cout << commands << endl;
    } else if (*input == "validatemap") {
      validateMap();
    } else if (*input == "addplayer") {
      addPlayer(input->getArg());
    } else if (*input == "gamestart") {
      if (state != GameState::PLAYERS_ADDED) {
        cout << wrongStateTransitionMessage << endl;
        continue;
      }

      if (players.size() < 2) {
        cout << "You can't have a game of less than 2 players, unless you're schizophrenic" << endl;
        continue;
      }

      auto rng = std::default_random_engine();
      std::shuffle(players.begin(), players.end(), rng);

      assignCountries();

      for (auto p : players) {
        p->hand->draw();
        p->hand->draw();
      }

      mainGameLoop();

      state = GameState::WIN;
    } else if (*input == "quit") {
      if (state != GameState::WIN) {
        cout << wrongStateTransitionMessage << endl;
        continue;
      }

      return;
    } else if (*input == "replay") {
      if (state != GameState::WIN) {
        cout << wrongStateTransitionMessage << endl;
        continue;
      }

      return;
    } else {
      cout << "Type the right command" << endl;
    }
    cout << "-----------------------------" << endl;
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

  state = MAP_LOADED;
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

  state = MAP_VALIDATED;
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

  state = PLAYERS_ADDED;
}

/**
 * Assign the countries like the teacher wants
 */
void GameEngine::assignCountries() {
  auto i = 0;
  // Territories are randomly shuffled by the Map class, so this operation is valid
  for (auto t : map->getAllTerritories()) {
    t->setOwner(players[i]);
    i = (i + 1) % players.size();
  }
}

/**
 * Destructor for the game engine
 */
GameEngine::~GameEngine() {
  _instance = nullptr;
  for (auto p : players) {
    delete p;
  }
  delete deck;
  delete map;
}

GameEngine::GameState GameEngine::getState() {
    return state;
}

GameEngine::GameState GameEngine::setState(GameEngine::GameState newState) {
    return state = newState;
}