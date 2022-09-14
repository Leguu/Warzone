//
// Created by Legu on 2022-09-13.
//

#ifndef WARZONE_PLAYER_H
#define WARZONE_PLAYER_H

class Player;

#include <vector>
#include "../Map/Map.h"

class Player {
public:
    const std::string name;
    std::vector<Territory *> ownedTerritories;
    int reinforcements;

    Player(std::string name);

    const std::vector<Territory *> toAttack();

    const std::vector<Territory *> toDefend();
};

#endif //WARZONE_PLAYER_H
