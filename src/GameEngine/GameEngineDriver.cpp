//
// Created by danie on 2022-10-08.
//

#include "GameEngine.h"

void testStartupPhase() {
  auto ge = new GameEngine();

  auto playerOneName = "Bob";
  auto playerTwoName = "Alice";

  std::vector<std::pair<std::string, std::string>>
	  testCommands =
	  {{"loadmap", "Moon"}, {"validatemap", string()}, {"addplayer", playerOneName}, {"addplayer", playerTwoName},
	   {"gamestart", string()}};

  auto gameStarted = !ge->startupPhase(testCommands);

  //  1) use the loadmap <filename> command to select a map from a list of map files as stored in a directory,
  //  which results in the map being loaded in the game.
  Utils::assertCondition(ge->map != nullptr, "loadmap command failed: did not load map");

  //  2) use the validatemap command to validate the map (i.e. it is a connected graph, etc – see assignment 1).
  Utils::assertCondition(ge->map->validate(), "validatemap command failed: could not validate map");

  auto v = ge->players;
  auto itPlayerOne = find_if(v.begin(), v.end(), [&playerOneName](Player *p) { return p->name == playerOneName; });
  auto itPlayerTwo = find_if(v.begin(), v.end(), [&playerTwoName](Player *p) { return p->name == playerTwoName; });

  //  3) use the addplayer <playername> command to enter players in the game (2-6 players)
  Utils::assertCondition(itPlayerOne != v.end() && itPlayerTwo != v.end(),
						 "addplayer command failed: could not add all players");

  //  4) use the gamestart command to
  //  	a) fairly distribute all the territories to the players
  Utils::assertCondition(ge->players[0]->ownedTerritories.size() == ge->players[1]->ownedTerritories.size(),
						 "gamestart command failed: unable to fairly distribute all the territories to the players");

  //  	b) determine randomly the order of play of the players in the game
  Utils::assertCondition(ge->players[0]->name == playerOneName || ge->players[1]->name == playerOneName,
						 "gamestart command failed: unable to randomly determine the order of play of the players in the game");

  //  	c) give 50 initial army units to the players, which are placed in their respective reinforcement pool
  Utils::assertCondition(ge->players[0]->reinforcements == 50 && ge->players[1]->reinforcements == 50,
						 "gamestart command failed: unable to give 50 initial army units to the players, which are placed in their respective reinforcement pool");

  //  	d) let each player draw 2 initial cards from the deck using the deck’s draw() method
  Utils::assertCondition(ge->players[0]->hand->cards.size() == 2 && ge->players[1]->hand->cards.size() == 2,
						 "gamestart command failed: each player should have drawn 2 initial cards from the deck");

  //  	e) switch the game to the play phase
  Utils::assertCondition(gameStarted, "gamestart command failed: game not in play phase");

  delete ge;
}

void testMainGameLoop() {
  auto ge = new GameEngine("../assets/Moon.map");
  auto playerOne = new Player("Bob");
  auto playerTwo = new Player("Alice");

  const auto territoryPlayerOne = ge->map->findTerritory("Bay of Dew");
  const auto territoriesPlayerTwo = {ge->map->findTerritory("Sea of Nectar"),
									 ge->map->findTerritory("Byrgius")};

  auto card = new AirliftCard();
  playerOne->hand->cards.push_back(card);
  playerOne->ownedTerritories.push_back(territoryPlayerOne);

  for (auto territory : territoryPlayerOne->getAdjTerritories()) {
	if (std::find(territoriesPlayerTwo.begin(), territoriesPlayerTwo.end(), territory) == territoriesPlayerTwo.end()) {
	  playerOne->ownedTerritories.push_back(territory);
	}
  }

  for (auto territoryPlayerTwo : territoriesPlayerTwo) {
	playerTwo->ownedTerritories.push_back(territoryPlayerTwo);
  }

  ge->players.push_back(playerOne);
  ge->players.push_back(playerTwo);

  Utils::assertCondition(playerOne->reinforcements == 50, "Wrong number of initial reinforcements assigned");
  Utils::assertCondition(playerTwo->reinforcements == 50, "Wrong number of initial reinforcements assigned");

  //  (1) a player receives the correct number of army units in the reinforcement phase (showing different cases);
  ge->reinforcementPhase();
  Utils::assertCondition((playerOne->reinforcements == 50 + 3),
						 "Wrong number of reinforcements were added for player one in the reinforcement phase");
  Utils::assertCondition((playerTwo->reinforcements == 50 + 2),
						 "Wrong number of reinforcements were added for player two in the reinforcement phase");

  //  (2) a player will only issue deploy orders and no other kind of orders if they still have army units in their reinforcement pool;
  ge->debugMode = true;
  ge->issueOrdersPhase();
  Utils::assertCondition(playerOne->reinforcementsAfterDeploy == 0, "Player one did not deploy all reinforcements");
  Utils::assertCondition(playerTwo->reinforcementsAfterDeploy == 0, "Player two did not deploy all reinforcements");

  // Since we don't have a way to track of past reinforcements after issuing an order,
  // we just make sure the first 'n' orders are "Deploy".
  //
  // Valid:   [x x x x y z] or [x x y y z y] or [x x x x x x]
  // Invalid: [y y y y y y] or [x x y y x y] or []
  // 		  where x is Deploy order.
  int i = 0;
  bool deployOrdersFirstP1 = false;
  while (i < playerOne->orders->getOrdersSize() && playerOne->orders->get(i)->name == "Deploy") {
	deployOrdersFirstP1 = true;
	i++;
  }

  for (; i < playerOne->orders->getOrdersSize(); i++) {
	if (playerOne->orders->get(i)->name == "Deploy") {
	  deployOrdersFirstP1 = false;
	}
  }

  i = 0;
  bool deployOrdersFirstP2 = false;
  while (i < playerTwo->orders->getOrdersSize() && playerTwo->orders->get(i)->name == "Deploy") {
	deployOrdersFirstP2 = true;
	i++;
  }

  for (; i < playerTwo->orders->getOrdersSize(); i++) {
	if (playerTwo->orders->get(i)->name == "Deploy") {
	  deployOrdersFirstP2 = false;
	}
  }

  Utils::assertCondition(deployOrdersFirstP1, "Player one did not issue deploy orders first");
  Utils::assertCondition(deployOrdersFirstP2, "Player two did not issue deploy orders first");

  //  (3) a player can issue advance orders to either defend or attack, based on the toAttack() and toDefend() lists
  i = 0;
  int advanceOrderCount = 0;
  bool advanceOrderPossibleP1 = false;
  for (; i < playerOne->orders->getOrdersSize(); i++) {
	if (playerOne->orders->get(i)->name == "Advance") {
	  auto target = dynamic_cast<AdvanceOrder *>((playerOne->orders->get(i)));
	  auto attack = playerOne->toAttack();
	  auto defend = playerOne->toDefend();
	  if (std::find(attack.begin(), attack.end(), target->getTarget()) != attack.end()
		  || std::find(defend.begin(), defend.end(), target->getTarget()) != defend.end()) {
		advanceOrderPossibleP1 = true;
	  }
	  advanceOrderCount++;
	}
  }

  i = 0;
  bool advanceOrderPossibleP2 = false;
  for (; i < playerTwo->orders->getOrdersSize(); i++) {
	if (playerTwo->orders->get(i)->name == "Advance") {
	  auto target = dynamic_cast<AdvanceOrder *>((playerTwo->orders->get(i)));
	  auto attack = playerTwo->toAttack();
	  auto defend = playerTwo->toDefend();
	  if (std::find(attack.begin(), attack.end(), target->getTarget()) != attack.end()
		  || std::find(defend.begin(), defend.end(), target->getTarget()) != defend.end()) {
		advanceOrderPossibleP2 = true;
	  }
	  advanceOrderCount++;
	}
  }

  if (advanceOrderCount == 0) {
	std::cout
		<< "No advance orders were issued by any player this time. This is because player actions are random. Test result cannot be determined."
		<< std::endl;
  } else {
	Utils::assertCondition(advanceOrderPossibleP1 || advanceOrderPossibleP2,
						   "Players did not issue advance orders from attack or defend list");
  }

  //  (4) a player can play cards to issue orders;
  i = 0;
  int cardOrderCount = 0;
  for (; i < playerOne->orders->getOrdersSize(); i++) {
	auto orderName = playerOne->orders->get(i)->name;
	if (orderName != "Bomb" || orderName != "Blockade" || orderName != "Airlift" || orderName != "Negotiate") {
	  cardOrderCount++;
	}
  }

  if (cardOrderCount == 0) {
	std::cout
		<< "No card order was issued by player one this time. This is because player actions are random. Test result cannot be determined."
		<< std::endl;
  } else {
	Utils::assertCondition(playerOne->hand->cards.empty(),
						   "Player one can not play their card");
  }

//  playerTwo->ownedTerritories = {};
//  ge->executeOrdersPhase();
  //  (5) TODO: a player that does not control any territory is removed from the game;
  //  (6) TODO: the game ends when a single player controls all the territories.

  cout << "All tests passed successfully!" << endl;
  delete ge;
}
