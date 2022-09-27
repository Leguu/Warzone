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
    // NOTE FOR SELF PLEASE READ
    // 1- Have to add loops to verify user input in CardsPlay functions
    // 2- Verify territories adjacency as if statement too?
    std::cout << divider << std::endl;
    std::cout << "Testing Cards.cpp functions" << std::endl;
    std::cout << divider << std::endl;
//    CardsTester::testBombCardConstructor();
//    CardsTester::testBombCardPlay();
//    CardsTester::testBlockadeCardConstructor();
//    CardsTester::testBlockadeCardPlay();
//    CardsTester::testAirliftCardConstructor();
//    CardsTester::testAirliftCardPlay();
//    CardsTester::testNegotiateCardConstructor();
//    CardsTester::testNegotiateCardPlay();
//    CardsTester::testHandConstructor();
//    CardsTester::testListHand();
//    CardsTester::testHandRemove();
//    CardsTester::testDeckConstructor();
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
    auto ge = new GameEngine("../assets/Moon.map");
    ge->deck = new Deck(cards);
    auto *player = new Player("bob");
    player->hand = new Hand();
    while (ge->deck->getCardsSize() != 0) {
        player->hand->draw();
    }
    for (auto *card: player->hand->cards) {
        player->play(card->name);
    }
    std::cout << "Content of Deck after every card has been played: " << ge->deck << std::endl;
    std::cout << "Content of the hand after card has been played: " << player->hand << std::endl;
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
    auto *player = new Player("Bob");
    std::vector<Card *> cards = {new BombCard()};
    auto ge = new GameEngine("../assets/Moon.map");
    ge->deck = new Deck(cards);
    auto *card = player->hand->draw();
    auto territory = new Territory("potato", "potato");
    auto *bombOrder = new BombOrder(player, territory);
    auto orderSizeBeforePlay = player->orders->getOrdersSize();
    player->play(card->name);
    auto orderSizeAfterPlay = player->orders->getOrdersSize();
    Utils::assert(orderSizeBeforePlay + 1 == orderSizeAfterPlay, "testBombCardPlay");
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
    auto *player = new Player("Bob");
    std::vector<Card *> cards = {new BlockadeCard()};
    auto ge = new GameEngine("../assets/Moon.map");
    ge->deck = new Deck(cards);
    auto *card = player->hand->draw();
    auto territory = new Territory("potato", "potato");
    territory->setOwner(player);
    ge->map->addTerritoryToMap(territory);
    std::cout << "The ID of the newly created territory used for this example is: " << territory->getId() << std::endl;
    auto orderSizeBeforePlay = player->orders->getOrdersSize();
    player->play(card->name);
    auto orderSizeAfterPlay = player->orders->getOrdersSize();
    Utils::assert(orderSizeBeforePlay + 1 == orderSizeAfterPlay, "testBlockadeCardPlay");

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
    auto *player = new Player("Bob");
    std::vector<Card *> cards = {new AirliftCard()};
    auto ge = new GameEngine("../assets/Moon.map");
    ge->deck = new Deck(cards);
    auto *card = player->hand->draw();
    auto territory = new Territory("potato", "potato");
    auto territoryTwo = new Territory("potato", "potato");
    territory->setOwner(player);
    territory->setArmies(10);
    ge->map->addTerritoryToMap(territory);
    territoryTwo->setOwner(player);
    ge->map->addTerritoryToMap(territoryTwo);
    std::cout << "The ID of the newly created \"from\" territory used for this example is: " << territory->getId() << std::endl;
    std::cout << "The ID of the newly created \"to\" territory used for this example is: " << territoryTwo->getId() << std::endl;
    auto orderSizeBeforePlay = player->orders->getOrdersSize();
    player->play(card->name);
    auto orderSizeAfterPlay = player->orders->getOrdersSize();
    Utils::assert(orderSizeBeforePlay + 1 == orderSizeAfterPlay, "testAirliftCardPlay");
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
    auto *player = new Player("Bob");
    auto *playerTwo = new Player("CoolerBob");
    std::vector<Card *> cards = {new NegotiateCard()};
    auto ge = new GameEngine("../assets/Moon.map");
    ge->deck = new Deck(cards);
    auto *card = player->hand->draw();
    ge->players.push_back(player);
    ge->players.push_back(playerTwo);
    std::cout << "The name of the newly created target player used for this example is: " << playerTwo->name << std::endl;
    auto orderSizeBeforePlay = player->orders->getOrdersSize();
    player->play(card->name);
    auto orderSizeAfterPlay = player->orders->getOrdersSize();
    Utils::assert(orderSizeBeforePlay + 1 == orderSizeAfterPlay, "testNegotiateCardPlay");
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
    auto ge = new GameEngine("../assets/Moon.map");
    ge->deck = new Deck(cards);
    auto initialDeckSize = ge->deck->getCardsSize();
    auto *card = ge->deck->draw();
    auto drawnDeckSize = ge->deck->getCardsSize();
    Utils::assert(initialDeckSize == drawnDeckSize + 1, "testDeckDraw");
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
    Utils::assert(initialDeckSize == addedDeckSize - 1, "testDeckPut");
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
    auto ge = new GameEngine("assets/Moon.map");
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
    auto ge = new GameEngine("assets/Moon.map");
    ge->deck = deck;
    while (deck->getCardsSize() != 0) {
        hand->draw();
    }
    auto addedHandSize = hand->cards.size();
    hand->remove(1);
    auto removedHandSize = hand->cards.size();
    Utils::assert(addedHandSize == removedHandSize + 1, "testHandRemove");
}
