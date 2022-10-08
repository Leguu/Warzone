//
// Created by danie on 2022-10-08.
//

#include "Player.h"
#include "../GameEngine/GameEngine.h"
#include <iostream>


void testPlayers() {
    auto ge = new GameEngine();
    ge->map = MapLoader::importMap("../assets/Moon.map");

    auto bob = new Player("Bob");

    auto john = new Player("John");

    ge->map->findTerritory("Byrgius")->setOwner(bob);
    ge->map->findTerritory("Bay of Dew")->setOwner(john);

    Utils::assertCondition(!bob->toAttack().empty(), "to attack not empty");

    Utils::assertCondition(!bob->toDefend().empty(), "to defend not empty");

    bob->issueOrder();

    delete ge;
}
