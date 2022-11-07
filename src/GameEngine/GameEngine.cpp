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
 * GameEngine constructor used to create a singleton game engine used to manage the game
 * @param mp the map
 */
GameEngine::GameEngine(const std::string &mp) {
    map = MapLoader::importMap(mp);
    _instance = this;
    this->Attach(LogObserver::instance());
}

/// Execute all the orders
/// \return whether win condition has been reached

bool GameEngine::executeOrdersPhase() {
    cout << endl << "Executing orders..." << endl;
    for (auto &player: this->players) {
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
    cout << "Next round has started" << endl << endl;
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
    for (auto player: players) {
        cout << "It is your turn to play: " << player->name << endl;
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
    }
}

void GameEngine::startupPhase() {
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
        if (Utils::isEqualLowercase(inputText.substr(0, inputText.find(' ')), "loadmap")) {
            loadMap("../assets/" + input->getArg() + ".map");
        } else if (*input == "help") {
            cout << commands << endl;
        } else if (Utils::isEqualLowercase(inputText, "validatemap")) {
            validateMap();
        } else if (Utils::isEqualLowercase(inputText.substr(0, inputText.find(' ')), "addplayer")) {
            addPlayer(input->getArg());
        } else if (Utils::isEqualLowercase(inputText, "gamestart")) {
            if (state != GameState::PLAYERS_ADDED) {
                cout << wrongStateTransitionMessage << endl;
                continue;
            }

            if (players.size() < 2) {
                cout << "You can't have a game of less than 2 players, unless you're schizophrenic" << endl;
                continue;
            }

            auto rng = std::default_random_engine();
            rng.seed(std::chrono::system_clock::now().time_since_epoch().count());
            std::shuffle(players.begin(), players.end(), rng);

            assignCountries();

            for (auto p: players) {
                p->hand->draw();
                p->hand->draw();
            }

            mainGameLoop();

            transition(GameState::WIN);
        } else if (Utils::isEqualLowercase(input->getCommand(), "quit")) {
            if (state != GameState::WIN) {
                cout << wrongStateTransitionMessage << endl;
                continue;
            }

            return;
        } else if (Utils::isEqualLowercase(input->getCommand(), "replay")) {
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
    auto i = 0;
    // Territories are randomly shuffled by the Map class, so this operation is valid
    for (auto t: map->getAllTerritories()) {
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
    file << "Game State Modified: " << this->gameStates[this->state] << std::endl << std::endl;
    return "Game State Modified: " + this->gameStates[this->state];
}

GameEngine::GameState GameEngine::getState() {
    return state;
}

void GameEngine::transition(GameEngine::GameState newState) {
    this->state = newState;
    this->Notify(this);
}
