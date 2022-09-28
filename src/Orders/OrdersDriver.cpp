#include <iostream>
#include <vector>
#include "../GameEngine/GameEngine.h"

#include "Order.h"

using namespace std;

void testOrdersLists() {
  OrderList list = OrderList();

  auto ge = new GameEngine("assets/Moon.map");
  auto territoryS = ge->map->findById(0);
  auto territoryT = ge->map->findById(1);
  auto player = Player("Bob");

  auto bomb = new BombOrder(&player, territoryT);
  auto block = new BlockadeOrder(&player, territoryT);
  auto air = new AirliftOrder(&player, 5, territoryS, territoryT); // int for armies
  auto adv = new AdvanceOrder(&player, 5, territoryS, territoryT);

  list.push(bomb);
}

int main() {
  std::cout << "Testing Orders Lists:" << std::endl;

  testOrdersLists();

  return 0;
}