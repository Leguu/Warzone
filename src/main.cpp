#include <iostream>
#include "Cards/Card.h"
#include "GameEngine.h"

int main() {
    auto ge = new GameEngine("assets/Moon.map");

    auto bombCard = new BombCard();

    auto territory = ge->map->findById(0);

    std::cout << "Armies of " + territory->name + " before: " << territory->armies << std::endl;

    bombCard->play();

    ge->executeOrders();

    std::cout << "Armies of " + territory->name + " after: " << territory->armies << std::endl;
}
