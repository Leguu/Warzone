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

    virtual void issueOrder(bool debugMode = false) = 0;

    virtual std::vector<std::pair<Territory *, Territory *>> toAttack() const = 0;

    virtual vector<Territory *> toDefend() const = 0;
};

class HumanStrategy : public PlayerStrategy {
public:
    explicit HumanStrategy(Player *pPlayer);

    [[nodiscard]] std::vector<std::pair<Territory *, Territory *>> toAttack() const override;

    [[nodiscard]] vector<Territory *> toDefend() const override;

    void issueOrder(bool debugMode = false) override;

private:
    void issueDeployOrder(bool debugMode = false);

    void issueAdvanceOrder(bool debugMode = false);

    void issueCardOrder(bool debugMode = false);
};

class AggressivePlayer : public PlayerStrategy {
public:
    explicit AggressivePlayer(Player *pPlayer);

    [[nodiscard]] std::vector<std::pair<Territory *, Territory *>> toAttack() const override;

    [[nodiscard]] vector<Territory *> toDefend() const override;

    void issueOrder(bool debugMode = false) override;

private:
    virtual void issueDeployOrder(bool debugMode = false);

    virtual void issueAdvanceOrder(bool debugMode = false);

    virtual void issueCardOrder(bool debugMode = false);
};

class CheaterStrategy : public AggressivePlayer {
public:
    explicit CheaterStrategy(Player *pPlayer);


private:
    void issueAdvanceOrder(bool debugMode = false);
};

class NeutralStrategy : public AggressivePlayer {
public:
    explicit NeutralStrategy(Player *pPlayer);

    void issueOrder(bool debugMode = false) override;

    void setHasBeenAttacked();

    bool getHasBeenAttacked();

private:
    bool hasBeenAttacked;
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

    vector<Player *> cannotAttack = vector<Player *>();

    bool isDoneIssuing = false;

    explicit Player(string name);

    vector<Territory *> getAdjacentEnemyTerritories();

    void issueOrder(bool debugMode = false);

    friend std::ostream &operator<<(std::ostream &os, const Player &player);

    ~Player();
};

void testPlayers();

#endif// WARZONE_PLAYER_H
