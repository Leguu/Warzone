//
// Created by danie on 2022-10-08.
//

#include "Order.h"
#include "../GameEngine/GameEngine.h"
#include <iostream>


void testOrdersLists() {
    OrderList list = OrderList();

    auto ge = new GameEngine("../assets/Moon.map");
    const auto territoryS = ge->map->findTerritory("Byrgius");
    const auto territoryT = ge->map->findTerritory("Bay of dew");
    auto player1 = Player("Bob");
    auto player2 = Player("Alice");

    auto deploy = new DeployOrder(&player1, 0, territoryT);
    auto advance = new AdvanceOrder(&player1, 5, territoryS, territoryT);
    auto bomb = new BombOrder(&player1, territoryT);
    auto blockade = new BlockadeOrder(&player1, territoryT);
    auto airlift = new AirliftOrder(&player1, 0, territoryS, territoryT);
    auto negotiate = new NegotiateOrder(&player1, &player2);

    list.push(deploy);
    list.push(advance);
    list.push(bomb);
    list.push(blockade);
    list.push(airlift);
    list.push(negotiate);
    Utils::assertCondition(list.getOrdersSize() == 6, "Push operation failed: list size is not 6.");

    list.remove(2);
    Utils::assertCondition(list.getOrdersSize() == 5, "Remove operation failed: list size is not 5.");
    Utils::assertCondition(list.get(2) == blockade, "Remove operation failed: index 2 is not blockade order.");

    list.move(1, 2);
    Utils::assertCondition(list.get(1) == blockade, "Move operation failed: index 1 is not blockade order.");
    Utils::assertCondition(list.get(2) == advance, "Move operation failed: index 2 is not advance order.");

    list.executeOrders();
    Utils::assertCondition(list.getOrdersSize() == 0, "Execute orders operation failed: list is not empty.");

    cout << "All tests passed successfully!" << endl;
    delete ge;
}
