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
  int reinforcements = 0;

  explicit Player(string name);

  vector<Territory *> getAdjacentEnemyTerritories();

  // TODO
  vector<Territory *> toAttack();

  // TODO
  vector<Territory *> toDefend();

  void issueOrder();

  void issueAdvanceOrder();

  void issueDeployOrder();

  void drawFromDeck() const;

  friend std::ostream &operator<<(std::ostream &os, const Player &player);

  virtual ~Player();
};

void testPlayers();

#endif //WARZONE_PLAYER_H
