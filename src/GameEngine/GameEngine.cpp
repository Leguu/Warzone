#include <iostream>
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

GameEngine::GameEngine(const std::string &mp) {
  map = MapLoader::importMap(mp);
  _instance = this;
}

/// @returns Game Over
bool GameEngine::executeOrders() {
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

Player *GameEngine::findPlayerByName(const std::string &name) {
  for (auto *player : this->players) {
	if (player->name == name) {
	  return player;
	}
  }
  return nullptr;
}
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
GameEngine::GameEngine() {
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
  _instance = this;
}

GameEngine *GameEngine::initialiseGame() {
  auto ge = new GameEngine();

  ge->initialisePlayers();

  return ge;
}

void GameEngine::issueOrders() {
  for (auto player : players) {
	cout << player->name << ", it is your turn!" << endl;
	player->issueOrder();
  }
}
