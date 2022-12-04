#ifndef WARZONE_GAMEENGINE_H
#define WARZONE_GAMEENGINE_H

class GameEngine;

#include "../CommandProcessor/CommandProcessor.h"
#include "../Logging/LoggingObserver.h"
#include "../Orders/Order.h"
#include "../Player/Player.h"
#include <memory>
#include <queue>

using std::cout;
using std::endl;
using std::runtime_error;
using std::string;
using std::vector;

class GameEngine : public ILoggable, public Subject {
  public:
  enum GameState {
    START,
    MAP_LOADED,
    MAP_VALIDATED,
    PLAYERS_ADDED,
    ASSIGN_REINFORCEMENTS,
    WIN
  };

  static inline string gameStates[6] = {"START",
                                        "MAP_LOADED",
                                        "MAP_VALIDATED",
                                        "PLAYERS_ADDED",
                                        "ASSIGN_REINFORCEMENTS",
                                        "WIN"};

  bool debugMode = false;
  bool testDynamicStrategyFlag = false;

  vector<Player *> players = vector<Player *>();
  Deck *deck =
          new Deck({new BombCard, new BombCard, new BombCard, new BombCard, new BombCard, new BombCard, new AirliftCard, new AirliftCard,
                    new AirliftCard, new AirliftCard, new AirliftCard, new AirliftCard,
                    new BlockadeCard, new BlockadeCard, new BlockadeCard, new NegotiateCard});
  CommandProcessor *commandProcessor = new CommandProcessor();

  int turnsGone = 0;

  Map *map = nullptr;

  static GameEngine *instance();

  Player *findPlayerByName(const std::string &name);

  explicit GameEngine(const std::string &mapPath);

  void reinforcementPhase() const;

  void issueOrdersPhase();

  void executeOrdersPhase();

  const static string helpText;

  bool startupPhase();

  void mainGameLoop();

  GameEngine();

  virtual ~GameEngine();

  GameState getState();

  std::string stringToLog() override;

  void transition(GameEngine::GameState newState);

  private:
  const static string wrongStateTransitionMessage;

  static GameEngine *_instance;
  GameState state = START;

  void loadMap(const string &input);

  void validateMap();

  void addPlayer(const string &playerName);

  void assignCountries();

  friend void testLoggingObserver();
  void tournamentMode(Command *pCommand);

  int gamesToRun = 1;
  int maxTurns = 100;
};

#endif// WARZONE_GAMEENGINE_H
