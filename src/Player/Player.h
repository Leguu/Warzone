#ifndef WARZONE_PLAYER_H
#define WARZONE_PLAYER_H

class Player;

#include "../Cards/Card.h"
#include "../Map/Map.h"
#include "../Orders/Order.h"
#include <memory>
#include <utility>
#include <vector>

class InvalidCardException : public std::runtime_error {
public:
  explicit InvalidCardException(const std::string &arg);
};

class Player {
public:
  const string name;
  OrderList *orders;
  Hand *hand;
  vector<Territory *> ownedTerritories = {};
  int reinforcements = 50;

  int reinforcementsAfterDeploy = 50;

  bool advanceOrderIssued = false;

  bool cardOrderIssued = false;

  int cardAwarded = false;

  vector<Player *> cannotAttack = vector<Player *>();

  bool isDoneIssuing = false;

  explicit Player(string name);

  vector<Territory *> getAdjacentEnemyTerritories();

  [[nodiscard]] std::vector<std::pair<Territory *, Territory *>> toAttack() const;

  [[nodiscard]] vector<Territory *> toDefend() const;

  void issueDeployOrder(bool debugMode = false);

  void issueAdvanceOrder(bool debugMode = false);

  void issueCardOrder(bool debugMode = false);

  void issueOrder(bool debugMode = false);

  void drawFromDeck() const;

  friend std::ostream &operator<<(std::ostream &os, const Player &player);

  ~Player();
};

void testPlayers();

#endif // WARZONE_PLAYER_H
