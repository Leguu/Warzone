//
// Created by Legu on 2022-09-13.
//

#ifndef WARZONE_GAMEENGINE_H
#define WARZONE_GAMEENGINE_H

class GameEngine;

#include <queue>
#include "Player/Player.h"
#include "Orders/Order.h"

class GameEngine {
public:
    static const bool debug = true;

    const std::vector<Player> players;
    const Map *map;

    std::queue<Order *> orders;

    static GameEngine *instance() {
        return _instance;
    }

    explicit GameEngine(const std::string &mapPath);

    void addPlayer(std::string name);

    void startup();

    void play();

    void executeOrders();
private:
    static GameEngine *_instance;
};


#endif //WARZONE_GAMEENGINE_H
