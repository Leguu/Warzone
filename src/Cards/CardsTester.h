//
// Created by danie on 2022-09-18.
//

#ifndef WARZONE_CARDSTESTER_H
#define WARZONE_CARDSTESTER_H

#include "./Card.h"
#include "../Player/Player.h"


class CardsTester {
private:
    void testBombCardConstructor();
    void testBombCardPlay();
    void testBlockadeCardConstructor();
    void testBlockadeCardPlay();
    void testAirliftCardConstructor();
    void testAirliftCardPlay();
    void testNegotiateCardConstructor();
    void testNegotiateCardPlay();
    void testHandConstructor();
    void testListHand();
    void testHandAdd();
    void testHandRemove();
    void testDeckConstructor();
    void testDeckDraw();
    void testDeckPut();
    void testDeckToString();
    void testHandToString();
    void testBombCardToString();
    void testBlockadeCardToString();
    void testAirliftCardToString();
    void testNegotiateCardToString();
};


#endif //WARZONE_CARDSTESTER_H
