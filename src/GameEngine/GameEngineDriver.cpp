//
// Created by danie on 2022-10-08.
//

#include "GameEngineDriver.h"
#include "GameEngine.h"

int main(){
    GameEngineDriver::testGameStates();
}

void GameEngineDriver::testGameStates() {
    auto ge = new GameEngine();
    ge->stupidGameLoopThatTheProfWants();
}
