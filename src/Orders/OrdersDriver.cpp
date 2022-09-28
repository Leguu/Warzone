#include <iostream>
#include <vector>
#include "../GameEngine/GameEngine.h"

#include "Order.h"

using namespace std;

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
  auto airlift = new AirliftOrder(&player1, 5, territoryS, territoryT);
  auto negotiate = new NegotiateOrder(&player1, &player2);

  list.push(deploy);
  list.push(advance);
  list.push(bomb);
  list.push(blockade);
  list.push(airlift);
  list.push(negotiate);

  cout << "\nPush operation test:" << endl;
  cout << "--------------------" << endl;
  cout << "Operation – 6 push commands" << endl;
  cout << "Expected – a list with 6 orders" << endl;
  cout << "Results – contents of list after operation was performed" << endl;
  cout << list << endl;

  cout << "\nRemove operation test:" << endl;
  cout << "---------------------" << endl;
  list.remove(2);
  cout << "Operation – remove performed on order number 3 (BombOrder, index 2)" << endl;
  cout << "Expected – the same list as above but with 5 orders" << endl;
  cout << "Results – contents of list after operation was performed" << endl;
  cout << list << endl;

  cout << "\nMove operation test:" << endl;
  cout << "---------------------" << endl;
  list.move(1, 2);
  cout
	  << "Operation – move performed between order number 2 and 3 (AdvanceOrder, index 1 and BlockadeOrder, index 2)"
	  << endl;
  cout << "Expected – the same list as above but with 5 orders" << endl;
  cout << "Results – contents of list after operation was performed" << endl;
  cout << list << endl;

}

int main() {
  cout << "---------------------" << endl;
  cout << "Testing Orders Lists:" << endl;
  cout << "---------------------" << endl;

  testOrdersLists();

  return 0;
}