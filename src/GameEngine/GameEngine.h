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
    enum GameState {
        START,
        MAP_LOADED,
        MAP_VALIDATED,
        PLAYERS_ADDED,
        ASSIGN_REINFORCEMENTS,
        WIN
    };


    inline static string toString(GameState g)
    {
        switch (g)
        {
            case START:   return "START";
            case MAP_LOADED:   return "MAP_LOADED";
            case MAP_VALIDATED: return "MAP_VALIDATED";
            case PLAYERS_ADDED:   return "PLAYERS_ADDED";
            case ASSIGN_REINFORCEMENTS: return "ASSIGN_REINFORCEMENTS";
            case WIN: return "WIN";
            default:      return "UNKNOWN_STATE";
        }
    }
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

    GameState getState();
    GameState setState(GameState);

private:

  const static string wrongStateTransitionMessage;

  static GameEngine *_instance;
  GameState state = START;

  void loadMap(const string &input);
  void validateMap();
  void addPlayer(const string &playerName);
  void assignCountries();
};
#endif //WARZONE_GAMEENGINE_H
