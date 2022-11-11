//
// Created by danie on 2022-11-02.
//

#include "LogObserver.h"
#include "LoggingObserver.h"
#include <iostream>
using std::cout;
#include "../CommandProcessor/CommandProcessor.h"
#include "../GameEngine/GameEngine.h"
#include "../Map/Map.h"
#include "../Orders/Order.h"
#include "../Player/Player.h"

void testLoggingObserver() {
  auto *log = new LogObserver();

  cout << "Part 1: Calling Notify and StringToLog on all classes to prove they "
		  "are children of Subject and Iloggable"
	   << endl;
  auto player1 = Player("Bob");
  auto ge = new GameEngine("../assets/Moon.map");
  const auto territoryS = ge->map->findTerritory("Byrgius");
  ge->state = GameEngine::START;
  ge->Notify(ge);
  ge->stringToLog();

  auto *command = new Command();
  command->command = "Part 1 Command";
  command->effect = "Part 1 Effect";
  command->Notify(command);
  command->stringToLog();

  auto *commandProcessor = new CommandProcessor();
  commandProcessor->commands.push_back(command);
  commandProcessor->Notify(commandProcessor);
  commandProcessor->stringToLog();

  auto *order = new BombOrder(&player1, territoryS);
  order->Notify(order);
  order->stringToLog();

  auto *orderList = new OrderList();
  orderList->orders.push_back(order);
  orderList->Notify(orderList);
  orderList->stringToLog();

  cout << "Part 2: Testing the Logging for all required methods" << endl;

  ge->transition(GameEngine::START);
  command->command = "Part 2 Command";
  command->saveEffect("Part 2 Effect");
  commandProcessor->saveCommand(command);
  orderList->push(order);
  order->execute();

  delete command;
  delete commandProcessor;
  delete order;
  delete orderList;
  delete ge;
  delete log;
};