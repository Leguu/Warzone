#include <iostream>
#include <vector>
#include "../GameEngine/GameEngine.h"

#include "Order.h"

void testOrdersLists() {
  OrderList list = OrderList();

  auto ge = new GameEngine("assets/Moon.map");
  const auto territoryS = ge->map->findById(0);
  const auto territoryT = ge->map->findById(1);
  auto player1 = Player("Bob");
  auto player2 = Player("Alice");

  auto deploy = new DeployOrder(&player1, 5, territoryT);
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

  std::cout << "\nPush operation test:" << std::endl;
  std::cout << "--------------------" << std::endl;
  std::cout << "Operation - 6 push commands" << std::endl;
  std::cout << "Expected - a list with 6 orders" << std::endl;
  std::cout << "Results - contents of list after operation was performed" << std::endl;
  std::cout << list << std::endl;

  std::cout << "\nRemove operation test:" << std::endl;
  std::cout << "---------------------" << std::endl;
  list.remove(2);
  std::cout << "Operation - remove performed on order number 3 (BombOrder, index 2)" << std::endl;
  std::cout << "Expected - the same list as above but with 5 orders" << std::endl;
  std::cout << "Results - contents of list after operation was performed" << std::endl;
  std::cout << list << std::endl;

  std::cout << "\nMove operation test:" << std::endl;
  std::cout << "---------------------" << std::endl;
  list.move(1, 2);
  std::cout
	  << "Operation – move performed between order number 2 and 3 (AdvanceOrder, index 1 and BlockadeOrder, index 2)"
	  << std::endl;
  std::cout << "Expected - the same list as above but with 5 orders" << std::endl;
  std::cout << "Results - contents of list after operation was performed" << std::endl;
  std::cout << list << std::endl;

  std::cout << "\nExecute operation test:" << std::endl;
  std::cout << "---------------------" << std::endl;
  std::cout << "Executing every order left in the list" << std::endl;
  std::cout << "Expected - every order is validated first and then executed" << std::endl;
  std::cout << "Results - contents of list after operation was performed" << std::endl;
  std::cout << "--------------------------------------------------------------------------------" << std::endl;
  list.executeOrders();
}

int main() {
  std::cout << "---------------------" << std::endl;
  std::cout << "Testing Orders Lists:" << std::endl;
  std::cout << "---------------------" << std::endl;

  testOrdersLists();

  return 0;
}