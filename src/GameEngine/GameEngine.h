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
  vector<Player *> players = vector<Player *>();
  Deck *deck = new Deck();

  Map *map = nullptr;

  static GameEngine *instance();

  void initialiseGame();

  Player *findPlayerByName(const std::string &name);

  explicit GameEngine(const std::string &mapPath);

  void runGameLoop();

  void assignReinforcements();

  void issueOrders();

  bool executeOrders();

  const static string helpText;

  // Dumb

  void stupidGameLoopThatTheProfWants();

  GameEngine();
private:
  enum GameState {
    START,
    MAP_LOADED,
    MAP_VALIDATED,
    PLAYERS_ADDED,
    ASSIGN_REINFORCEMENTS,
    ISSUE_ORDERS,
    EXECUTE_ORDERS,
    WIN
  };

  const static string wrongStateTransitionMessage;

  static GameEngine *_instance;
  GameState state = START;

  void initialisePlayers();
  void stupidLoadMap(const string &input);
  void stupidValidateMap();
  void stupidAddPlayer(const string &playerName);
  void stupidAssignCountries();
};
#endif //WARZONE_GAMEENGINE_H
