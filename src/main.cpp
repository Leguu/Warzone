#include <iostream>
#include "Cards/Card.h"
#include "GameEngine/GameEngine.h"

// TODO Test functions
int main() {
    auto ge = new GameEngine("assets/Moon.map");

    auto bombCard = new BombCard();

    auto territory = ge->map->findById(0);

    ge->players.push_back(std::make_unique<Player>("Bob"));

    std::cout << "Armies of " + territory->getName() + " before: " << territory->getArmies() << std::endl;

    bombCard->play(ge->players[0].get());

    ge->executeOrders();

    std::cout << "Armies of " + territory->getName() + " after: " << territory->getArmies() << std::endl;
}
