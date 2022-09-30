#ifndef WARZONE_GAMEENGINE_H
#define WARZONE_GAMEENGINE_H

class GameEngine;

#include <queue>
#include <memory>
#include "../Player/Player.h"
#include "../Orders/Order.h"

class GameEngine {
public:
  static const bool debug = true;

  std::vector<Player *> players;
  Map *map;

  static GameEngine *instance() {
    return _instance;
  }

  // TODO
  Player *findPlayerByName(std::string name);

  explicit GameEngine(const std::string &mapPath);

  void play();

  // TODO: Ask for the number of users, and input names, and id starting territory.
  inline void initialisePlayers() {}

  // TODO: Go through all the players and figure out what number of reinforcements they deserve.
  inline void assignReinforcements() {}

  // TODO: Ask every player to issue an order, or play some cards.
  inline void issueOrders() {}

  /// This is a command
  void listOrders() {}

  bool executeOrders();

private:
  static GameEngine *_instance;
};

#endif //WARZONE_GAMEENGINE_H
