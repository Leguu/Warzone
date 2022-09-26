//
// Created by danie on 2022-09-24.
//

#include "CardsDriver.h"
#include "./Card.h"
#include "../Player/Player.h"
#include "../GameEngine/GameEngine.h"
#include "../Utils/Utils.h"
#include <iostream>


int main() {
/* ============================================================================================
 * Testing Cards Class Elements
 * ============================================================================================
 * */
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

/**
 * Verify that we can create a valid deck, draw all its cards, play them, and everything is functional
 */
void testCards() {
    std::vector<Card *> cards = {new BombCard(), new AirliftCard(), new BombCard(), new NegotiateCard,
                                 new NegotiateCard, new BlockadeCard, new BlockadeCard};
    auto *deck = new Deck(cards);
    auto *hand = new Hand();
    auto *player = new Player("bob");
    while (deck->getCardsSize() != 0) {
        hand->draw();
    }
    for (auto *card: hand->cards) {
        player->play(card->name);
    }
    std::cout << "Content of Deck after every card has been played: " << deck << std::endl;
    std::cout << "Content of the hand after card has been played: " << hand << std::endl;
}

/**
 * Test the BombCard constructor
 */
void CardsTester::testBombCardConstructor() {
    auto *card = new BombCard();
    std::cout << "Bomb Card built successfully : name : " << card->name << ", desc: " << card->description << std::endl;
}

/**
 * Test the BombCard play function
 */
void CardsTester::testBombCardPlay() {
    auto *card = new BombCard();
    auto *player = new Player("Bob");
    auto territory = new Territory("potato", "potato");
    auto *bombOrder = new BombOrder(player, territory);
    player->play(card->name);
    auto order = player->orders->pop();
    Utils::assert(typeid(order).name() == typeid(bombOrder).name(), "testBombCardPlay");
}

/**
 * Test the BlockadeCard constructor
 */
void CardsTester::testBlockadeCardConstructor() {
    auto *card = new BlockadeCard();
    std::cout << "Blockade Card built successfully : name : " << card->name << ", desc: " << card->description
              << std::endl;
}

/**
 * Test the Blockade Card play function
 */
void CardsTester::testBlockadeCardPlay() {
    auto *card = new BlockadeCard();
    auto *player = new Player("Bob");
    auto territory = new Territory("potato", "potato");
    auto *blockadeOrder = new BlockadeOrder(player, territory);
    player->play(card->name);
    auto order = player->orders->pop();
    Utils::assert(typeid(order).name() == typeid(blockadeOrder).name(), "testBlockadeCardPlay");
}

/**
 * Test the AirliftCard constructor
 */
void CardsTester::testAirliftCardConstructor() {
    auto *card = new AirliftCard();
    std::cout << "Airlift Card built successfully : name : " << card->name << ", desc: " << card->description
              << std::endl;
}

/**
 * Test the AirliftCard play function
 */
void CardsTester::testAirliftCardPlay() {
    auto *card = new AirliftCard();
    auto *player = new Player("Bob");
    auto territory = new Territory("potato", "potato");
    auto *airliftOrder = new AirliftOrder(player, 2, territory, territory);
    player->play(card->name);
    auto order = player->orders->pop();
    Utils::assert(typeid(order).name() == typeid(AirliftOrder).name(), "testAirliftCardPlay");
}

/**
 * Test the NegotiateCard constructor
 */
void CardsTester::testNegotiateCardConstructor() {
    auto *card = new NegotiateCard();
    std::cout << "Negotiate Card built successfully : name : " << card->name << ", desc: " << card->description
              << std::endl;
}

/**
 * Test the NegotiateCard play function
 */
void CardsTester::testNegotiateCardPlay() {
    auto *card = new NegotiateCard();
    auto *player = new Player("Bob");
    auto *negotiateOrder = new NegotiateOrder(player, player);
    player->play(card->name);
    auto order = player->orders->pop();
    Utils::assert(typeid(order).name() == typeid(negotiateOrder).name(), "testNegotiateCardPlay");
}

/**
 * Test the Hand constructor
 */
void CardsTester::testHandConstructor() {
    auto *hand = new Hand();
    std::cout << "Hand successfully built, should be empty. Number of cards held in hand :" << hand->cards.size()
              << std::endl;
}

/**
 * Test the Hand Listhand function
 */
void CardsTester::testListHand() {
    auto *hand = new Hand();
    hand->listHand();
    std::cout << "Finished printing Hand" << std::endl;
}

/**
 * Test the Deck constructor
 */
void CardsTester::testDeckConstructor() {
    std::vector<Card *> cards = {new BombCard(), new AirliftCard(), new BombCard(), new NegotiateCard,
                                 new BlockadeCard};
    auto *deck = new Deck(cards);
    std::cout << "Deck successfully built, should contain a n>0 number of cards: " << deck->getCardsSize() << std::endl;
}

/**
 * Test the Deck draw function
 */
void CardsTester::testDeckDraw() {
    std::vector<Card *> cards = {new BombCard(), new AirliftCard(), new BombCard(), new NegotiateCard,
                                 new BlockadeCard};
    auto *deck = new Deck(cards);
    auto initialDeckSize = deck->getCardsSize();
    auto *card = deck->draw();
    auto drawnDeckSize = deck->getCardsSize();
    Utils::assert(initialDeckSize == drawnDeckSize - 1, "testDeckDraw");
}

/**
 * Test the Deck put function
 */
void CardsTester::testDeckPut() {
    std::vector<Card *> cards = {new BombCard(), new AirliftCard(), new BombCard(), new NegotiateCard,
                                 new BlockadeCard};
    auto *deck = new Deck(cards);
    auto *card = new BlockadeCard();
    auto initialDeckSize = deck->getCardsSize();
    deck->put(card);
    auto addedDeckSize = deck->getCardsSize();
    Utils::assert(initialDeckSize == addedDeckSize + 1, "testDeckPut");
}

/**
 * Test the Deck toString function
 */
void CardsTester::testDeckToString() {
    std::vector<Card *> cards = {new BombCard(), new AirliftCard(), new BombCard(), new NegotiateCard,
                                 new BlockadeCard};
    auto *deck = new Deck(cards);
    std::cout << deck << std::endl;
    std::cout << "Finished printing Deck" << std::endl;
}

/**
 * Test the Hand toString function
 */
void CardsTester::testHandToString() {
    std::vector<Card *> cards = {new BombCard(), new AirliftCard(), new BombCard(), new NegotiateCard,
                                 new BlockadeCard};
    auto *deck = new Deck(cards);
    auto *hand = new Hand();
    auto ge = GameEngine::instance();
    ge->deck = deck;
    while (deck->getCardsSize() != 0) {
        hand->draw();
    }
    std::cout << hand << std::endl;
    std::cout << "Finished printing Hand" << std::endl;
}

/**
 * Test the BombCard toString function
 */
void CardsTester::testBombCardToString() {
    auto *card = new BombCard();
    std::cout << card;
    std::cout << "Finished printing BombCard" << std::endl;
}

/**
 * Test the BlockadeCard toString function
 */
void CardsTester::testBlockadeCardToString() {
    auto *card = new BlockadeCard();
    std::cout << card;
    std::cout << "Finished printing BlockadeCard" << std::endl;
}

/**
 * Test the AirliftCard toString function
 */
void CardsTester::testAirliftCardToString() {
    auto *card = new AirliftCard();
    std::cout << card;
    std::cout << "Finished printing AirliftCard" << std::endl;

}

/**
 * Test the NegotiateCard toString function
 */
void CardsTester::testNegotiateCardToString() {
    auto *card = new NegotiateCard();
    std::cout << card;
    std::cout << "Finished printing NegotiateCard" << std::endl;
}

/**
 * Test the Hand remove function
 */
void CardsTester::testHandRemove() {
    std::vector<Card *> cards = {new BombCard(), new AirliftCard(), new BombCard(), new NegotiateCard,
                                 new BlockadeCard};
    auto *deck = new Deck(cards);
    auto *hand = new Hand();
    auto ge = GameEngine::instance();
    ge->deck = deck;
    while (deck->getCardsSize() != 0) {
        hand->draw();
    }
    auto addedHandSize = hand->cards.size();
    hand->remove(1);
    auto removedHandSize = hand->cards.size();
    Utils::assert(addedHandSize == removedHandSize + 1, "testHandRemove");
}
