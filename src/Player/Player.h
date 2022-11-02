#ifndef WARZONE_PLAYER_H
#define WARZONE_PLAYER_H

class Player;

#include <utility>
#include <vector>
#include <memory>
#include "../Map/Map.h"
#include "../Orders/Order.h"
#include "../Cards/Card.h"
using std::vector;
using std::cout;
using std::endl;
using std::string;

class Player {
public:
  const string name;
  OrderList *orders;
  Hand *hand;
  vector<Territory *> ownedTerritories = {};
  int reinforcements = 50;

  explicit Player(string name);

  vector<Territory *> getAdjacentEnemyTerritories();

  [[nodiscard]] vector<Territory *> toAttack() const;

  [[nodiscard]] vector<Territory *> toDefend() const;

  void issueOrder();

  void issueAdvanceOrder();

  void issueDeployOrder();

  void drawFromDeck() const;

  friend std::ostream &operator<<(std::ostream &os, const Player &player);

  ~Player();
};

void testPlayers();

#endif //WARZONE_PLAYER_H
