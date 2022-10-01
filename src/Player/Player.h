#ifndef WARZONE_PLAYER_H
#define WARZONE_PLAYER_H

class Player;

#include <utility>
#include <vector>
#include <memory>
#include "../Map/Map.h"
#include "../Orders/Order.h"
#include "../Cards/Card.h"

class Hand;
class OrderList;

class Player {
public:
    const std::string name;
    OrderList *orders = new OrderList();
    Hand *hand = new Hand();
    std::vector<Territory *> ownedTerritories = {};
    int reinforcements = 0;

    inline explicit Player(std::string name) : name(std::move(name)) {}

    std::vector<Territory*> getAdjacentEnemyTerritories();

    // TODO
    const std::vector<Territory *> toAttack();

    // TODO
    const std::vector<Territory *> toDefend();

    // TODO
    void issueOrder();

    void drawFromDeck() const;

    void play(std::string const &cardName);

    friend std::ostream &operator<<(std::ostream &os, const Player &player);

private:
    [[nodiscard]] Card* findCardByName(std::string name) const;
};

#endif //WARZONE_PLAYER_H
