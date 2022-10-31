#ifndef WARZONE_GAMEENGINE_H
#define WARZONE_GAMEENGINE_H

class GameEngine;

#include <queue>
#include <memory>
#include "../Player/Player.h"
#include "../Orders/Order.h"
#include "../CommandProcessor/CommandProcessor.h"
using std::runtime_error;
using std::string;
using std::vector;
using std::cout;
using std::endl;

class GameEngine {
public:
  vector<Player *> players = vector<Player *>();
  Deck *deck = new Deck({new BombCard, new BombCard, new AirliftCard, new AirliftCard, new BlockadeCard,
                                         new BlockadeCard, new NegotiateCard});
  CommandProcessor *commandProcessor = new CommandProcessor();

  Map *map = nullptr;

  static GameEngine *instance();

  Player *findPlayerByName(const std::string &name);

  explicit GameEngine(const std::string &mapPath);

  void reinforcementPhase() const;

  void issueOrdersPhase();

  bool executeOrdersPhase();

  const static string helpText;

  void startupPhase();

  void mainGameLoop();

  GameEngine();

  virtual ~GameEngine();
private:
  enum GameState {
    START,
    MAP_LOADED,
    MAP_VALIDATED,
    PLAYERS_ADDED,
    ASSIGN_REINFORCEMENTS,
    WIN
  };

  const static string wrongStateTransitionMessage;

  static GameEngine *_instance;
  GameState state = START;

  void loadMap(const string &input);
  void validateMap();
  void addPlayer(const string &playerName);
  void assignCountries();
};
#endif //WARZONE_GAMEENGINE_H
