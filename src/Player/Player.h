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

    virtual void issueOrder() = 0;

    virtual std::vector<std::pair<Territory *, Territory *>> toAttack() const = 0;

    virtual vector<Territory *> toDefend() const = 0;

    virtual bool isDoneIssuing() = 0;
};

class DefaultPlayerStrategy : public PlayerStrategy {
public:
    DefaultPlayerStrategy(Player *pPlayer);

    [[nodiscard]] std::vector<std::pair<Territory *, Territory *>> toAttack() const override;

    [[nodiscard]] vector<Territory *> toDefend() const override;

    void issueOrder() override;

    bool isDoneIssuing() override;

private:
    void issueDeployOrder();

    void issueAdvanceOrder();

    void issueCardOrder();
};

<<<<<<< Updated upstream
=======
class BenevolentPlayer : public PlayerStrategy {
  public:
  explicit BenevolentPlayer(Player *pPlayer);

  [[nodiscard]] std::vector<std::pair<Territory *, Territory *>> toAttack() const override;

  [[nodiscard]] vector<Territory *> toDefend() const override;

  void issueOrder() override;

  bool isDoneIssuing() override;

  private:
  virtual void issueDeployOrder();

  virtual void issueAdvanceOrder();

  virtual void issueCardOrder();
};


>>>>>>> Stashed changes
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
