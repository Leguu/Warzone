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

class Hand;
class OrderList;

class Player {
public:
  const string name;
  OrderList *orders = new OrderList();
  Hand *hand = new Hand();
  vector<Territory *> ownedTerritories = {};
  int reinforcements = 0;

  inline explicit Player(string name) : name(std::move(name)) {}

  vector<Territory *> getAdjacentEnemyTerritories();

  // TODO
  vector<Territory *> toAttack();

  // TODO
  vector<Territory *> toDefend();

  void issueOrder();

  void issueAdvanceOrder();

  void issueDeployOrder();

  void drawFromDeck() const;

  void play(string const &cardName);

  friend std::ostream &operator<<(std::ostream &os, const Player &player);

private:
  [[nodiscard]] Card *findCardByName(const string& cardName) const;
};

#endif //WARZONE_PLAYER_H
