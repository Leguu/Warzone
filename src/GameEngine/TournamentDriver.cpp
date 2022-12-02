//
// Created by danie on 2022-12-02.
//
#include "GameEngine.h"
#include <iostream>

void testTournament(){
        auto ge = new GameEngine();
        ge->debugMode = true;
        std::cout << "Default Game Recommendation: tournament -M moon -P Benevolent Aggressive -G 2 -D 200" << std::endl;
        ge->startupPhase();
        delete ge;
        std::cout << "Tournament mode executed properly. Verify tournament output log for proof" << std::endl;
};
