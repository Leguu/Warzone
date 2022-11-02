#include <iostream>
#include <fstream>
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
bool GameEngine::executeOrders() {
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
    return false;
}

/**
 * Game loop to run the game
 */
void GameEngine::runGameLoop() {
    cout << "Welcome to Warzone! Type in \"help\" at any time to have a list of commands" << endl;
    while (true) {
        assignReinforcements();

        issueOrders();

        auto gameOver = executeOrders();

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
 * Ask for all the players and initialize them
 */
void GameEngine::initialisePlayers() {
    cout << *map << endl;
    cout << "Input the name of every single player, and then type \"done\" when finished." << endl;
    cout << "You will also need to choose their starting territory. Use the map representation above to begin!" << endl;
    while (true) {
        auto name = Utils::getInputString("Input the player name");

        if (Utils::isEqualLowercase(name, "done") || name.empty()) {
            if (players.size() < 2) {
                cout << "You have to input at least 2 players" << endl;
                continue;
            }
            return;
        }

        auto player = new Player(name);

        try {
            while (true) {
                auto reinforcements = Utils::getInputInt("How many reinforcements will this player start with?", true);

                if (reinforcements < 0) {
                    cout << "Input a value greater than or equal to 0" << endl;
                    continue;
                }

                player->reinforcements = reinforcements;
                break;
            }
        } catch (Utils::CancelledInputException &) {
            continue;
        }

        Territory *territory;
        try {
            while (true) {
                territory = map->getInputTerritory("What territory does this player start in?", true);

                if (territory->getOwner()) {
                    cout << "That territory already has an owner... Try again" << endl;
                    continue;
                }

                break;
            }
        } catch (Utils::CancelledInputException &) {
            continue;
        }

        territory->setOwner(player);

        this->players.push_back(player);
        cout << "Player " << player->name << " starts off in territory " << territory->getName() << " with "
             << player->reinforcements
             << " reinforcements to start with." << endl;
    }
}

/**
 * Empty constructor to create the game engine singleton variable
 */
GameEngine::GameEngine() {
    _instance = this;
}

/**
 * Initialize the game
 */
void GameEngine::initialiseGame() {
    while (true) {
        auto path = Utils::getInputString("Input the path of the map. Leave empty for a default map.");

        if (path.empty()) {
            path = "../assets/Moon.map";
        }

        try {
            map = MapLoader::importMap(path);
            break;
        } catch (runtime_error &e) {
            cout << "There was an error with that map: " << e.what() << endl;
        }
    }

    initialisePlayers();
}

/**
 * Issue the orders
 */
void GameEngine::issueOrders() {
    for (auto player: players) {
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
void GameEngine::assignReinforcements() const {
    for (auto t: map->getAllTerritories()) {
        auto owner = t->getOwner();
        if (owner) {
            owner->reinforcements += 1;
        }
    }

    for (auto c: map->getContinents()) {
        auto owner = c->owner();
        if (owner) {
            owner->reinforcements += c->getBonus();
        }
    }
}

/**
 * Game loop the teacher wants
 */
void GameEngine::stupidGameLoopThatTheProfWants() {
    state = START;
    const string commands =
            "Available commands:\n"
            "LoadMap <Map Name> - Load a map\n"
            "ValidateMap - Validate the map has been loaded successfully\n"
            "AddPlayer <Name> - Add a player\n"
            "AssignCountries - Assign countries to each player in the game\n"
            "IssueOrder - Issue an order\n"
            "EndIssueOrders - Finish issuing orders\n"
            "EndExecOrders - End your turn and execute the orders\n";

    string input;

    cout << "Game has begun!" << endl;
    while (state != WIN) {
        input = Utils::getInputString("Input your command, write \"help\" for help");
        auto split = Utils::tokenizer(input, ' ');

        if (split.empty()) {
            return;
        }
        auto command = Utils::toLowercase(Utils::trim(split[0]));
        auto lowerCaseCommand = Utils::toLowercase(command);
        if (lowerCaseCommand == "loadmap") {
            stupidLoadMap("../assets/" + Utils::trim(input.substr(7)) + ".map");
        } else if (lowerCaseCommand == "help") {
            cout << commands << endl;
        } else if (lowerCaseCommand == "validatemap") {
            stupidValidateMap();
        } else if (lowerCaseCommand == "addplayer") {
            stupidAddPlayer(Utils::trim(input.substr(9)));
        } else if (lowerCaseCommand == "assigncountries") {
            stupidAssignCountries();
        } else if (lowerCaseCommand == "issueorder") {
            if (state != ASSIGN_REINFORCEMENTS) {
                cout << wrongStateTransitionMessage << endl;
                continue;
            }
            for (auto p: players) {
                p->issueOrder();
            }
            state = ISSUE_ORDERS;
        } else if (lowerCaseCommand == "endissueorders") {
            if (state != ISSUE_ORDERS) {
                cout << wrongStateTransitionMessage << endl;
                continue;
            }
            if (executeOrders()) {
                state = WIN;
                this->Notify(this);
            } else {
                state = EXECUTE_ORDERS;
                this->Notify(this);
            }
        } else if (lowerCaseCommand == "endexecorders") {
            if (state != EXECUTE_ORDERS) {
                cout << wrongStateTransitionMessage << endl;
                continue;
            }

            assignReinforcements();
            state = ASSIGN_REINFORCEMENTS;
            this->Notify(this);
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
void GameEngine::stupidLoadMap(const string &input) {
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
    this->Notify(this);
}

/**
 * Validate the map like the teacher wants
 */
void GameEngine::stupidValidateMap() {
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
    this->Notify(this);
}

/**
 * Add player like teacher wants
 * @param playerName The player name
 */
void GameEngine::stupidAddPlayer(const string &playerName) {
    if (state != MAP_VALIDATED && state != PLAYERS_ADDED) {
        cout << wrongStateTransitionMessage << endl;
        return;
    }

    if (playerName.empty()) {
        cout << "Can't add a player named nothing baka" << endl;
        return;
    }

    players.push_back(new Player(playerName));

    state = PLAYERS_ADDED;
    this->Notify(this);
}

/**
 * Assign the countries like the teacher wants
 */
void GameEngine::stupidAssignCountries() {
    if (state != PLAYERS_ADDED) {
        cout << wrongStateTransitionMessage << endl;
        return;
    }

    if (players.size() < 2) {
        cout << "Can't have a game with fewer than 2 players, unless you're schizophrenic" << endl;
        return;
    }

    for (auto p: players) {
        auto territory = map->getInputTerritory("Set the starting territory for player " + p->name, false);

        territory->setOwner(p);

        cout << "Territory set for player " << p->name << endl;
    }

    state = ASSIGN_REINFORCEMENTS;
    assignReinforcements();
    this->Notify(this);
}

/**
 * Log the information of this order
 * @return a String containing the information that will be logged
 */
std::string GameEngine::stringToLog() {
    std::ofstream file;
    file.open("../logs/gamelog.txt", std::ios_base::app);
    file << "Game State Modified: " << this->stateToString(this->state) << std::endl << std::endl;
    return "Game State Modified: " + this->stateToString(this->state);
}


/**
 * Destructor for the game engine
 */
GameEngine::~GameEngine() {
    _instance = nullptr;
    for (auto p: players) {
        delete p;
    }
    delete deck;
    delete map;
}
/**
 * Return the string value of each enum
 * @param gamestate The enum
 * @return Their string value
 */
std::string GameEngine::stateToString(const GameEngine::GameState gamestate) {
    switch (gamestate) {
        case GameState::START:
            return "START";
        case GameState::MAP_LOADED:
            return "MAP_LOADED";
        case GameState::MAP_VALIDATED:
            return "MAP_VALIDATED";
        case GameState::PLAYERS_ADDED:
            return "PLAYERS_ADDED";
        case GameState::ASSIGN_REINFORCEMENTS:
            return "ASSIGN_REINFORCEMENTS";
        case GameState::ISSUE_ORDERS:
            return "ISSUE_ORDERS";
        case GameState::EXECUTE_ORDERS:
            return "EXECUTE_ORDERS";
        case GameState::WIN:
            return "WIN";
    }
    return NULL;
}



