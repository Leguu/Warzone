#ifndef WARZONE_PLAYER_H
#define WARZONE_PLAYER_H

class Player;
class PlayerStrategy;

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

class PlayerStrategy {
  public:
  Player *p;

  explicit PlayerStrategy(Player *P);

  virtual void issueOrder();

  virtual std::vector<std::pair<Territory *, Territory *>> toAttack() const = 0;

  virtual vector<Territory *> toDefend() const = 0;

  virtual bool isDoneIssuing() = 0;

  private:
  virtual void issueDeployOrder() = 0;

  virtual void issueAdvanceOrder() = 0;

  virtual void issueCardOrder() = 0;
};

class DefaultPlayerStrategy : public PlayerStrategy {
  public:
  explicit DefaultPlayerStrategy(Player *pPlayer);

  [[nodiscard]] std::vector<std::pair<Territory *, Territory *>> toAttack() const override;

  [[nodiscard]] vector<Territory *> toDefend() const override;

  bool isDoneIssuing() override;

  private:
  void issueDeployOrder();

  void issueAdvanceOrder();

  void issueCardOrder();
};

class AggressivePlayerStrategy : public PlayerStrategy {
  public:
  explicit AggressivePlayerStrategy(Player *pPlayer);

  [[nodiscard]] std::vector<std::pair<Territory *, Territory *>> toAttack() const override;

  [[nodiscard]] vector<Territory *> toDefend() const override;

  bool isDoneIssuing() override;

  private:
  virtual void issueDeployOrder();

  virtual void issueAdvanceOrder();

  virtual void issueCardOrder();
};

class CheaterStrategy : public AggressivePlayerStrategy {
  public:
  explicit CheaterStrategy(Player *pPlayer);


  private:
  void issueAdvanceOrder();
};

class NeutralStrategy : public PlayerStrategy {
  public:
  explicit NeutralStrategy(Player *pPlayer);

  inline bool isDoneIssuing() override {return true};
  void issueOrder() override;
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

  friend std::ostream &operator<<(std::ostream &os, const Player &player);

  ~Player();

  [[nodiscard]] std::vector<std::pair<Territory *, Territory *>> toAttack() const;

  [[nodiscard]] vector<Territory *> toDefend() const;

  void issueOrder();

  bool isDoneIssuing();

  private:
  void issueDeployOrder();

  void issueAdvanceOrder();

  void issueCardOrder();
};

void testPlayers();

#endif// WARZONE_PLAYER_H
