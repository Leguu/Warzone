//
// Created by danie on 2022-10-08.
//

#include "../GameEngine/GameEngine.h"
#include "Player.h"
#include <iostream>

void testPlayers() {
  auto ge = new GameEngine();
  ge->map = MapLoader::importMap("../assets/Moon.map");

  auto bob = new Player("Bob");

  auto john = new Player("John");

  ge->map->findTerritory("Byrgius")->setOwner(bob);
  ge->map->findTerritory("Bay of Dew")->setOwner(john);

  bob->issueOrder();

  delete ge;
}

void testPlayerStrategies(){

  auto ge = new GameEngine("../assets/Moon.map");

  auto bob = new Player("Bob");

  auto john = new Player("John");

  bob->strategy = new AggressivePlayerStrategy(bob);
  john->strategy = new BenevolentPlayer(john);


  ge->map->findTerritory("Sea of Nectar")->setOwner(bob);
  ge->map->findTerritory("Ocean of Storms")->setOwner(john);
  ge->map->findTerritory("Sea of Clouds")->setOwner(bob);
  ge->map->findTerritory("Marsh of Diseases")->setOwner(john);
  ge->map->findTerritory("Harpalus")->setOwner(bob);
  ge->map->findTerritory("Rhaeticus")->setOwner(john);


  ge->players.push_back(bob);
  ge->players.push_back(john);

  ge->debugMode = true;
  ge->mainGameLoop();
  delete ge;
}