#ifndef WARZONE_GAMEENGINE_H
#define WARZONE_GAMEENGINE_H

class GameEngine;

#include <queue>
#include <memory>
#include "../Player/Player.h"
#include "../Orders/Order.h"
using std::runtime_error;
using std::string;
using std::vector;
using std::cout;
using std::endl;

class GameEngine {
public:
  static const bool debug = true;

  std::vector<Player *> players;
  Map *map;
  Deck *deck = new Deck();

  static GameEngine *instance() {
    if (!_instance)
      throw runtime_error("GameEngine instance not yet initialised. Did you forget to create a GameEngine first?");
    return _instance;
  }

  static GameEngine *initialiseGame();

  Player *findPlayerByName(const std::string &name);

  explicit GameEngine(const std::string &mapPath);

  void runGameLoop();

  // TODO: Go through all the players and figure out what number of reinforcements they deserve.
  inline void assignReinforcements() {}

  void issueOrders();

  bool executeOrders();

  const static string helpText;

private:
  static GameEngine *_instance;

  GameEngine();

  void initialisePlayers();
};
#endif //WARZONE_GAMEENGINE_H
