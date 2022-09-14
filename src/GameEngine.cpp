//
// Created by Legu on 2022-09-13.
//

#include <iostream>
#include "GameEngine.h"

GameEngine *GameEngine::_instance = nullptr;

GameEngine::GameEngine(const std::string &mp) {
    map = MapLoader::importMap(mp);
    _instance = this;
}

void GameEngine::executeOrders() {
    while (!orders.empty()) {
        auto order = orders.front();
        orders.pop();
        std::cout << *order << std::endl;
        order->execute();
    }
}
