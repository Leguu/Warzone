#include <iostream>
#include "Cards/Card.h"
#include "Cards/CardsDriver.h"
#include "GameEngine/GameEngine.h"

class CardsTester;

// TODO Test functions
int main() {
  auto ge = new GameEngine("../assets/Moon.map");
  ge->deck->put(new BombCard());

  auto territory = ge->map->findById(0);

  auto bob = new Player("Bob");
  auto john = new Player("John"); // an enemy

  ge->players.push_back(bob);
  ge->players.push_back(john);

  territory->getAdjTerritories()[0]->setOwner(bob);
  territory->setOwner(john);
  territory->setArmies(10);

  try {
    bob->drawFromDeck();
    bob->play("bomb");
  } catch (Utils::CancelledInputException &_e) {
    std::cout << "Input was cancelled." << std::endl;
  }

  std::cout << "Armies of " + territory->getName() + " before: " << territory->getArmies() << std::endl;

  ge->executeOrders();

  std::cout << "Armies of " + territory->getName() + " after: " << territory->getArmies() << std::endl;

}
