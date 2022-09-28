//
// Created by danie on 2022-09-24.
//

#ifndef WARZONE_CARDSDRIVER_H
#define WARZONE_CARDSDRIVER_H

#include "./Card.h"
#include "../Player/Player.h"


void testCards();

class CardsTester {
public:
    static void testBombCardConstructor();

    static void testBombCardPlay();

    static void testBlockadeCardConstructor();

    static void testBlockadeCardPlay();

    static void testAirliftCardConstructor();

    static void testAirliftCardPlay();

    static void testNegotiateCardConstructor();

    static void testNegotiateCardPlay();

    static void testHandConstructor();

    static void testListHand();

    static void testHandRemove();

    static void testDeckConstructor();

    static void testDeckDraw();

    static void testDeckPut();

    static void testDeckToString();

    static void testHandToString();

    static void testBombCardToString();

    static void testBlockadeCardToString();

    static void testAirliftCardToString();

    static void testNegotiateCardToString();
};

#endif //WARZONE_CARDSDRIVER_H
