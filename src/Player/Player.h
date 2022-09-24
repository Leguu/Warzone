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

class Player {
public:
    const std::string name;
    OrderList* orders = new OrderList();
    Hand* hand = new Hand();
    std::vector<Territory *> ownedTerritories = {};
    int reinforcements = 0;

    inline explicit Player(std::string name) : name(std::move(name)) {}

    const std::vector<Territory *> toAttack();

    const std::vector<Territory *> toDefend();

    void play(std::string &name);

    bool remove(const std::string& name);

    void draw();
};

#endif //WARZONE_PLAYER_H
