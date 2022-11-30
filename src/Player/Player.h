#ifndef WARZONE_PLAYER_H
#define WARZONE_PLAYER_H

class Player;
class PlayerStrategy;

#include "../Cards/Card.h"
#include "../Map/Map.h"
#include "../Orders/Order.h"
#include <map>
#include <memory>
#include <utility>
#include <vector>

class InvalidCardException : public std::runtime_error {
  public:
  explicit InvalidCardException(const std::string &arg);
};

class PlayerStrategy {
  public:
  Player *p;

  explicit PlayerStrategy(Player *P);

  virtual void issueOrder();

  [[nodiscard]] virtual std::vector<std::pair<Territory *, Territory *>> toAttack() const;

  [[nodiscard]] virtual vector<Territory *> toDefend() const = 0;

  virtual bool isDoneIssuing();

  private:
  virtual std::map<std::string, int> getCardNameMap() {
    return {{"Bomb", 0},
            {"Blockade", 1},
            {"Airlift", 2},
            {"NegotiateCard", 3}};
  }

  virtual void issueDeployOrder();

  virtual void issueAdvanceOrder() = 0;

  virtual void issueCardOrder();
};

class DefaultPlayerStrategy : public PlayerStrategy {
  public:
  explicit DefaultPlayerStrategy(Player *pPlayer);

  [[nodiscard]] std::vector<std::pair<Territory *, Territory *>> toAttack() const override;

  [[nodiscard]] vector<Territory *> toDefend() const override;

  void issueOrder() override;

  bool isDoneIssuing() override;

  private:
  void issueDeployOrder() override;

  void issueAdvanceOrder() override;

  void issueCardOrder() override;
};

class AggressivePlayerStrategy : public PlayerStrategy {
  public:
  explicit AggressivePlayerStrategy(Player *pPlayer);

  [[nodiscard]] std::vector<std::pair<Territory *, Territory *>> toAttack() const override;

  [[nodiscard]] vector<Territory *> toDefend() const override;

  private:
  std::map<std::string, int> getCardNameMap() override {
    return {
            {"Bomb", 0},
            {"Airlift", 2},
    };
  }

  int nextToAttack = 0;

  void issueAdvanceOrder() override;
};


class Player {
  public:
  const string name;
  OrderList *orders;
  Hand *hand;
  vector<Territory *> ownedTerritories = {};
  int reinforcements = 50;
  PlayerStrategy *strategy;

  int reinforcementsAfterDeploy = 50;

  bool advanceOrderIssued = false;

  bool cardOrderIssued = false;

  int cardAwarded = false;

  vector<Player *> cannotAttack = {};

  explicit Player(string name);

  vector<Territory *> getAdjacentEnemyTerritories();

  void issueOrder();

  friend std::ostream &operator<<(std::ostream &os, const Player &player);

  ~Player();
};

void testPlayers();

#endif// WARZONE_PLAYER_H
