#include <iostream>
#include "Cards/Card.h"
#include "Cards/CardsTester.h"
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
    CardsTester* ct = new CardsTester();
    ct->testBombCardConstructor();
    ct->testBombCardPlay();
    ct->testBlockadeCardConstructor();
    ct->testBlockadeCardPlay();
    ct->testAirliftCardConstructor();
    ct->testAirliftCardPlay();
    ct->testNegotiateCardConstructor();
    ct->testNegotiateCardPlay();
    ct->testHandConstructor();
    ct->testListHand();
    ct->testHandAdd();
    ct->testHandRemove();
    ct->testDeckConstructor();
    ct->testDeckDraw();
    ct->testDeckPut();
    ct->testDeckToString();
    ct->testHandToString();
    ct->testBombCardToString();
    ct->testBlockadeCardToString();
    ct->testAirliftCardToString();
    ct->testNegotiateCardToString();
}
