#include <iostream>
#include "Cards/Card.h"
#include "Cards/CardsDriver.h"
#include "GameEngine/GameEngine.h"

class CardsTester;

// TODO Test functions
int main() {
    auto ge = new GameEngine("../assets/Moon.map");

    auto bombCard = new BombCard();

    auto territory = ge->map->findById(0);

    ge->players.push_back(new Player("Bob"));
    territory->setArmies(10);

    bombCard->play(ge->players[0]);


    std::cout << "Armies of " + territory->getName() + " after: " << territory->getArmies() << std::endl;

    ge->executeOrders();

}
