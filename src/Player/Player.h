#ifndef WARZONE_PLAYER_H
#define WARZONE_PLAYER_H

class Player;

#include <utility>
#include <vector>
#include <memory>
#include "../Map/Map.h"
#include "../Orders/Order.h"
#include "../Cards/Card.h"

class Player {
public:
    const std::string name;
    std::unique_ptr<OrderList> orders;
    std::unique_ptr<CardManager> cardManager;
    std::vector<Territory *> ownedTerritories;
    int reinforcements = 0;

    inline explicit Player(std::string name) : name(std::move(name)) {
        orders = std::make_unique<OrderList>();
    }

    const std::vector<Territory *> toAttack();

    const std::vector<Territory *> toDefend();
};

#endif //WARZONE_PLAYER_H
