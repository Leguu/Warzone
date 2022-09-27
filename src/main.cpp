#include <iostream>
#include "Cards/Card.h"
#include "Cards/CardsDriver.h"
#include "GameEngine/GameEngine.h"

class CardsTester;

// TODO Test functions
int main() {
//    auto ge = new GameEngine("assets/Moon.map");
//
//    auto bombCard = new BombCard();
//
//    auto territory = ge->map->findById(0);
//
//    ge->players.push_back(new Player("Bob"));
//
//    std::cout << "Armies of " + territory->getName() + " before: " << territory->getArmies() << std::endl;
//
//    bombCard->play(ge->players[0]);
//
//    ge->executeOrders();
//
//    std::cout << "Armies of " + territory->getName() + " after: " << territory->getArmies() << std::endl;

    auto divider = "================================================================";
    /* ---------------------------- Testing All Methods ---------------------------- */
    std::cout << divider << std::endl;
    std::cout << "Testing Cards.cpp functions" << std::endl;
    std::cout << divider << std::endl;
    CardsTester::testBombCardConstructor();
    CardsTester::testBombCardPlay();
    CardsTester::testBlockadeCardConstructor();
    CardsTester::testBlockadeCardPlay();
    CardsTester::testAirliftCardConstructor();
    CardsTester::testAirliftCardPlay();
    CardsTester::testNegotiateCardConstructor();
    CardsTester::testNegotiateCardPlay();
    CardsTester::testHandConstructor();
    CardsTester::testListHand();
    CardsTester::testHandRemove();
    CardsTester::testDeckConstructor();
    CardsTester::testDeckDraw();
    CardsTester::testDeckPut();
    CardsTester::testDeckToString();
    CardsTester::testHandToString();
    CardsTester::testBombCardToString();
    CardsTester::testBlockadeCardToString();
    CardsTester::testAirliftCardToString();
    CardsTester::testNegotiateCardToString();

    /* ---------------------------- Testing TestCards Method ---------------------------- */
    std::cout << divider << std::endl;
    std::cout << "Cards Driver : TestCards method" << std::endl;
    std::cout << divider << std::endl;
    testCards();

}
