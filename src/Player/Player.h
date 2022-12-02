#ifndef WARZONE_PLAYER_H
#define WARZONE_PLAYER_H

#include "../Cards/Card.h"
#include "../Map/Map.h"
#include "../Orders/Order.h"
#include <map>
#include <memory>
#include <utility>
#include <vector>

class PlayerStrategy {
  public:
  Player *p;

  explicit PlayerStrategy(Player *P);

  virtual void issueOrder();

  [[nodiscard]] virtual std::vector<std::pair<Territory *, Territory *>> toAttack() const;

  [[nodiscard]] virtual vector<Territory *> toDefend() const = 0;

  virtual bool isDoneIssuing();

  bool advanceOrderIssued = false;

  bool cardOrderIssued = false;

  virtual void resetTurn();

  virtual void onAttack();

  virtual vector<string> allowedCards();

  private:
  virtual void issueAdvanceOrder() = 0;

  protected:
  virtual void issueDeployOrder();
  virtual void issueCardOrder();
};

class DefaultPlayerStrategy : public PlayerStrategy {
  public:
  explicit DefaultPlayerStrategy(Player *pPlayer);

  [[nodiscard]] vector<Territory *> toDefend() const override;

  void issueOrder() override;

  private:
  void issueAdvanceOrder() override;
};

class AggressivePlayerStrategy : public PlayerStrategy {
  public:
  explicit AggressivePlayerStrategy(Player *pPlayer);

  [[nodiscard]] std::vector<std::pair<Territory *, Territory *>> toAttack() const override;

  [[nodiscard]] vector<Territory *> toDefend() const override;

  vector<string> allowedCards() override;

  private:

  int nextToAttack = 0;

  void issueAdvanceOrder() override;
};

class CheaterStrategy : public PlayerStrategy {
  public:
  explicit CheaterStrategy(Player *pPlayer);

  bool isDoneIssuing() override;

  void issueOrder() override;

  [[nodiscard]] vector<Territory *> toDefend() const override;

  private:
  void issueDeployOrder() override;

  void issueAdvanceOrder() override;

  void issueCardOrder() override;
};

class NeutralStrategy : public PlayerStrategy {
  public:
  explicit NeutralStrategy(Player *pPlayer);
  [[nodiscard]] vector<std::pair<Territory *, Territory *>> toAttack() const override;
  [[nodiscard]] vector<Territory *> toDefend() const override;

  void onAttack() override;

  private:
  void issueAdvanceOrder() override;

  public:
  inline bool isDoneIssuing() override { return true; }
  void issueOrder() override;
};

class HumanStrategy : public PlayerStrategy {
  public:
  explicit HumanStrategy(Player *p);
  void issueOrder() override;
  bool isDoneIssuing() override;

  [[nodiscard]] vector<std::pair<Territory *, Territory *>> toAttack() const override;
  [[nodiscard]] vector<Territory *> toDefend() const override;

  void resetTurn() override;

  private:
  bool doneIssuing = false;
  void issueDeployOrder() override;
  void issueAdvanceOrder() override;
  void issueCardOrder() override;
};

class BenevolentPlayer : public PlayerStrategy {
  public:
  explicit BenevolentPlayer(Player *pPlayer);

  [[nodiscard]] std::vector<std::pair<Territory *, Territory *>> toAttack() const override;

  [[nodiscard]] vector<Territory *> toDefend() const override;

  vector<string> allowedCards() override;

  private:

  void issueDeployOrder() override;
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

  bool cardAwarded = false;

  vector<Player *> cannotAttack = {};

  explicit Player(string name);

  vector<Territory *> getAdjacentEnemyTerritories();

  void issueOrder() const;

  friend std::ostream &operator<<(std::ostream &os, const Player &player);

  ~Player();
};

void testPlayers();

#endif// WARZONE_PLAYER_H
