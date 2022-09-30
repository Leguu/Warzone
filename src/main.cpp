#include <iostream>
#include "Cards/Card.h"
#include "GameEngine/GameEngine.h"

// TODO Test functions
int main() {
  auto ge = new GameEngine("../assets/Moon.map");

  auto bombCard = new BombCard();

  auto territory = ge->map->findById(0);

  territory->setArmies(10);

  auto bob = new Player("Bob");
  ge->players.push_back(bob);

  std::cout << "Armies of " + territory->getName() + " before: " << territory->getArmies() << std::endl;

  bombCard->play(bob);

  ge->executeOrders();

  std::cout << "Armies of " + territory->getName() + " after: " << territory->getArmies() << std::endl;
}
