#include "Cards/Card.h"
#include "Cards/CardsDriver.cpp"
#include "CommandProcessor/CommandProcessorDriver.cpp"
#include "GameEngine/GameEngine.h"
#include "GameEngine/GameEngineDriver.cpp"
#include "Logging/LogObserver.h"
#include "Logging/LoggingObserverDriver.cpp"
#include "Map/MapDriver.cpp"
#include "Orders/OrdersDriver.cpp"
#include "Player/PlayerDriver.cpp"
#include "Utils/Utils.h"

int main() {
  auto log = new LogObserver();
  while (true) {
	auto input = Utils::getInputString(
		"What do you want to test? cards, map, orders, players, logging, startup, mainloop, game "
		"command. Type quit to quit");

	if (Utils::isEqualLowercase(input, "cards")) {
	  testCards();
	} else if (Utils::isEqualLowercase(input, "map")) {
	  testLoadMaps();
	} else if (Utils::isEqualLowercase(input, "orders")) {
	  testOrdersLists();
	} else if (Utils::isEqualLowercase(input, "players")) {
	  testPlayers();
	} else if (Utils::isEqualLowercase(input, "logging")) {
	  testLoggingObserver();
	} else if (input == "command") {
	  testCommandProcessor();
	} else if (Utils::isEqualLowercase(input, "startup")) {
	  testStartupPhase();
	} else if (Utils::isEqualLowercase(input, "mainloop")) {
	  testMainGameLoop();
	} else if (Utils::isEqualLowercase(input, "game")) {
	  auto ge = new GameEngine();
	  ge->debugMode = true;

//	  std::vector<std::pair<std::string, std::string>>
//		  testCommands =
//		  {{"loadmap", "Moon"}, {"validatemap", string()}, {"addplayer", "A"}, {"addplayer", "B"}, {"addplayer", "C"},
//		   {"Gamestara", string()}};

//	    loadmap Moon
//	    validatemap
//	    addplayer A
//	    addplayer B
//	    addplayer C},
//		Gamestart
	  ge->startupPhase();

	  delete ge;
	} else if (Utils::isEqualLowercase(input, "quit")) {
	  break;
	} else {
	  cout << "Unrecognised command" << endl;
	}
  }
}
