//
// Created by danie on 2022-09-24.
//

#include "CardsDriver.h"
#include "./Card.h"
#include "../Player/Player.h"
#include "../GameEngine/GameEngine.h"
#include "../Utils/Utils.h"
#include <iostream>

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


void CardsTester::testBombCardConstructor() {
    auto *card = new BombCard();
    std::cout << "Bomb Card built successfully : name : " << card->name << ", desc: " << card->description << std::endl;
}

void CardsTester::testBombCardPlay() {
    auto *card = new BombCard();
    auto *player = new Player("Bob");
    auto territory = new Territory("potato");
    auto *bombOrder = new BombOrder(player, territory);
    player->play(card->name);
    auto order = player->orders->pop();
    Utils::assert(typeid(order).name() == typeid(bombOrder).name(), "testBombCardPlay");
}

void CardsTester::testBlockadeCardConstructor() {
    auto *card = new BlockadeCard();
    std::cout << "Blockade Card built successfully : name : " << card->name << ", desc: " << card->description
              << std::endl;
}

void CardsTester::testBlockadeCardPlay() {
    auto *card = new BlockadeCard();
    auto *player = new Player("Bob");
    auto territory = new Territory("potato");
    auto *blockadeOrder = new BlockadeOrder(player, territory);
    player->play(card->name);
    auto order = player->orders->pop();
    Utils::assert(typeid(order).name() == typeid(blockadeOrder).name(), "testBlockadeCardPlay");
}

void CardsTester::testAirliftCardConstructor() {
    auto *card = new AirliftCard();
    std::cout << "Airlift Card built successfully : name : " << card->name << ", desc: " << card->description
              << std::endl;
}

void CardsTester::testAirliftCardPlay() {
    auto *card = new AirliftCard();
    auto *player = new Player("Bob");
    auto territory = new Territory("potato");
    auto *airliftOrder = new AirliftOrder(player, 2, territory, territory);
    player->play(card->name);
    auto order = player->orders->pop();
    Utils::assert(typeid(order).name() == typeid(AirliftOrder).name(), "testAirliftCardPlay");
}

void CardsTester::testNegotiateCardConstructor() {
    auto *card = new NegotiateCard();
    std::cout << "Negotiate Card built successfully : name : " << card->name << ", desc: " << card->description
              << std::endl;
}

void CardsTester::testNegotiateCardPlay() {
    auto *card = new NegotiateCard();
    auto *player = new Player("Bob");
    auto *negotiateOrder = new NegotiateOrder(player, player);
    player->play(card->name);
    auto order = player->orders->pop();
    Utils::assert(typeid(order).name() == typeid(negotiateOrder).name(), "testNegotiateCardPlay");
}

void CardsTester::testHandConstructor() {
    auto *hand = new Hand();
    std::cout << "Hand successfully built, should be empty. Number of cards held in hand :" << hand->cards.size()
              << std::endl;
}

void CardsTester::testListHand() {
    auto *hand = new Hand();
    hand->listHand();
    std::cout << "Finished printing Hand" << std::endl;
}

void CardsTester::testDeckConstructor() {
    std::vector<Card *> cards = {new BombCard(), new AirliftCard(), new BombCard(), new NegotiateCard,
                                 new BlockadeCard};
    auto *deck = new Deck(cards);
    std::cout << "Deck successfully built, should contain a n>0 number of cards: " << deck->getCardsSize() << std::endl;
}

void CardsTester::testDeckDraw() {
    std::vector<Card *> cards = {new BombCard(), new AirliftCard(), new BombCard(), new NegotiateCard,
                                 new BlockadeCard};
    auto *deck = new Deck(cards);
    auto initialDeckSize = deck->getCardsSize();
    auto *card = deck->draw();
    auto drawnDeckSize = deck->getCardsSize();
    Utils::assert(initialDeckSize == drawnDeckSize - 1, "testDeckDraw");
}

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

void CardsTester::testDeckToString() {
    std::vector<Card *> cards = {new BombCard(), new AirliftCard(), new BombCard(), new NegotiateCard,
                                 new BlockadeCard};
    auto *deck = new Deck(cards);
    std::cout << deck << std::endl;
    std::cout << "Finished printing Deck" << std::endl;
}

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

void CardsTester::testBombCardToString() {
    auto *card = new BombCard();
    std::cout << card;
    std::cout << "Finished printing BombCard" << std::endl;
}

void CardsTester::testBlockadeCardToString() {
    auto *card = new BlockadeCard();
    std::cout << card;
    std::cout << "Finished printing BlockadeCard" << std::endl;
}

void CardsTester::testAirliftCardToString() {
    auto *card = new AirliftCard();
    std::cout << card;
    std::cout << "Finished printing AirliftCard" << std::endl;

}

void CardsTester::testNegotiateCardToString() {
    auto *card = new NegotiateCard();
    std::cout << card;
    std::cout << "Finished printing NegotiateCard" << std::endl;
}

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
