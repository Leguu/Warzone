#include "Cards/Card.h"
#include "GameEngine/GameEngine.h"

int main() {
  auto ge = new GameEngine();


//  ge->initialiseGame();
//  ge->runGameLoop();

  ge->stupidGameLoopThatTheProfWants();
}
