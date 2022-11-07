#include "GameEngine/GameEngine.h"
#include "Cards/Card.h"
#include "Orders/OrdersDriver.cpp"
#include "Player/PlayerDriver.cpp"
#include "Map/MapDriver.cpp"
#include "Cards/CardsDriver.cpp"
#include "GameEngine/GameEngineDriver.cpp"
#include "CommandProcessor/CommandProcessorDriver.cpp"

int main() {
  while (true) {
    auto
        input = Utils::getInputString("What do you want to test? cards, map, orders, players, game, command. Type quit to quit");

    if (input == "cards") {
      testCards();
    } else if (input == "map") {
      testLoadMaps();
    } else if (input == "orders") {
      testOrdersLists();
    } else if (input == "players") {
      testPlayers();
    } else if (input == "game") {
      auto ge = new GameEngine();
      ge->startupPhase();
    } else if (input == "command") {
      testCommandProcessor();
    } else if (input == "quit") {
      break;
    } else {
      cout << "Unrecognised command" << endl;
    }
  }
}
