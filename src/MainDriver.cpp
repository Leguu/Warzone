#include "GameEngine/GameEngine.h"
#include "Cards/Card.h"

int main() {
  while (true) {
    auto input = Utils::getInputString("What do you want to test? cards, map, orders, players, game. Type quit to quit");

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
      ge->stupidGameLoopThatTheProfWants();
    } else if (input == "quit") {
      break;
    } else {
      cout << "Unrecognised command" << endl;
    }
  }
}
