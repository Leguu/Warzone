#ifndef WARZONE_PLAYER_H
#define WARZONE_PLAYER_H

class Player;

#include "../Cards/Card.h"
#include "../Map/Map.h"
#include "../Orders/Order.h"
#include <memory>
#include <utility>
#include <vector>
using std::cout;
using std::endl;
using std::string;
using std::vector;

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

  int reinforcementsAfterDeploy = reinforcements;

  int cardAwarded = false;

  explicit Player(string name);

  vector<Territory *> getAdjacentEnemyTerritories();

  [[nodiscard]] vector<Territory *> toAttack() const;

  [[nodiscard]] vector<Territory *> toDefend() const;

  void issueDeployOrder();

  void issueAdvanceOrder();

  void issueCardOrder();

  bool issueOrder();

  void drawFromDeck() const;

  friend std::ostream &operator<<(std::ostream &os, const Player &player);

  ~Player();
};

void testPlayers();

#endif // WARZONE_PLAYER_H
