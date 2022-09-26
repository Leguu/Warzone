#include <iostream>
#include "Cards/Card.h"
#include "Cards/CardsDriver.h"
#include "GameEngine/GameEngine.h"

// TODO Test functions
int main() {
//    auto ge = new GameEngine("assets/Moon.map");
//
//    auto bombCard = new BombCard();
//
//    auto territory = ge->map->findById(0);
//
//    ge->players.push_back(std::make_unique<Player>("Bob"));
//
//    std::cout << "Armies of " + territory->name + " before: " << territory->armies << std::endl;
//
//    bombCard->play(ge->players[0].get());
//
//    ge->executeOrders();
//
//    std::cout << "Armies of " + territory->name + " after: " << territory-

/* ============================================================================================
 * Testing Cards Class Elements
 * ============================================================================================
 * */
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
}
