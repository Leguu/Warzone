//
// Created by daniel on 2022-09-18.
//

#include "CardsTester.h"
#include "./Card.h"
#include "../Player/Player.h"
#include <iostream>


void CardsTester::testBombCardConstructor() {
    auto* card = new BombCard();
    std::cout << "Bomb Card built successfully : name : " << card->name << ", desc: " << card->description << std::endl;
}

void CardsTester::testBombCardPlay() {
    auto* card = new BombCard();
    auto* player = new Player("Bob");
    auto handSizeInitially = player->hand->cards.size();
    card->play(player);
    auto handSizeAfterPlay = player->hand->cards.size();
    if (handSizeInitially == handSizeAfterPlay + 1){
        std::cout << "Successfully played the card: " << card << ". Original size of the hand: " << handSizeInitially << ", and size after card removed: " << handSizeAfterPlay <<std::endl;
    }
    else{
        std::cout << "Error playing the card. Size of the hand before and after card is played is not valid" << std::endl;
    }
}

void CardsTester::testBlockadeCardConstructor() {
    auto* card = new BlockadeCard();
    std::cout << "Blockade Card built successfully : name : " << card->name << ", desc: " << card->description << std::endl;
}

void CardsTester::testBlockadeCardPlay() {
    auto* card = new BlockadeCard();
    auto* player = new Player("Bob");
    auto handSizeInitially = player->hand->cards.size();
    card->play(player);
    auto handSizeAfterPlay = player->hand->cards.size();
    if (handSizeInitially == handSizeAfterPlay + 1){
        std::cout << "Successfully played the card: " << card << ". Original size of the hand: " << handSizeInitially << ", and size after card removed: " << handSizeAfterPlay <<std::endl;
    }
    else{
        std::cout << "Error playing the card. Size of the hand before and after card is played is not valid" << std::endl;
    }
}

void CardsTester::testAirliftCardConstructor() {
    auto* card = new AirliftCard();
    std::cout << "Airlift Card built successfully : name : " << card->name << ", desc: " << card->description << std::endl;
}

void CardsTester::testAirliftCardPlay() {
    auto* card = new AirliftCard();
    auto* player = new Player("Bob");
    auto handSizeInitially = player->hand->cards.size();
    card->play(player);
    auto handSizeAfterPlay = player->hand->cards.size();
    if (handSizeInitially == handSizeAfterPlay + 1){
        std::cout << "Successfully played the card: " << card << ". Original size of the hand: " << handSizeInitially << ", and size after card removed: " << handSizeAfterPlay <<std::endl;
    }
    else{
        std::cout << "Error playing the card. Size of the hand before and after card is played is not valid" << std::endl;
    }
}

void CardsTester::testNegotiateCardConstructor() {
    auto* card = new NegotiateCard();
    std::cout << "Negotiate Card built successfully : name : " << card->name << ", desc: " << card->description << std::endl;
}

void CardsTester::testNegotiateCardPlay() {
    auto* card = new NegotiateCard();
    auto* player = new Player("Bob");
    auto handSizeInitially = player->hand->cards.size();
    card->play(player);
    auto handSizeAfterPlay = player->hand->cards.size();
    if (handSizeInitially == handSizeAfterPlay + 1){
        std::cout << "Successfully played the card: " << card << ". Original size of the hand: " << handSizeInitially << ", and size after card removed: " << handSizeAfterPlay <<std::endl;
    }
    else{
        std::cout << "Error playing the card. Size of the hand before and after card is played is not valid" << std::endl;
    }
}

void CardsTester::testHandConstructor() {
    auto* hand = new Hand();
    std::cout << "Hand successfully built, should be empty. Number of cards held in hand :" << hand->cards.size() << std::endl;
}

void CardsTester::testListHand() {
    auto* hand = new Hand();
    hand->listHand();
    std::cout << "Finished printing Hand" << std::endl;
}

void CardsTester::testDeckConstructor() {
    std::vector<Card*> cards = {new BombCard(), new AirliftCard(), new BombCard(), new NegotiateCard, new BlockadeCard};
    auto* deck = new Deck(cards);
    std::cout << "Deck successfully built, should contain a n>0 number of cards: " << deck->cardCollection.size() << std::endl;
}

void CardsTester::testDeckDraw() {
    std::vector<Card*> cards = {new BombCard(), new AirliftCard(), new BombCard(), new NegotiateCard, new BlockadeCard};
    auto* deck = new Deck(cards);
    auto initialDeckSize = deck->cardCollection.size();
    auto* card = deck->draw();
    auto drawnDeckSize = deck->cardCollection.size();
    if (initialDeckSize == drawnDeckSize - 1){
        std::cout << "Successfully drew a card from the deck. Card drawn: " << card << ". Original size of the deck: " << initialDeckSize << ", and size after card drawn: " << drawnDeckSize <<std::endl;
    }
    else{
        std::cout << "Error drawing a card from the deck. Size of the deck before and after being drawn is not valid" << std::endl;
    }
}

void CardsTester::testDeckPut() {
    std::vector<Card*> cards = {new BombCard(), new AirliftCard(), new BombCard(), new NegotiateCard, new BlockadeCard};
    auto* deck = new Deck(cards);
    auto* card = new BlockadeCard();
    auto initialDeckSize = deck->cardCollection.size();
    deck->put(card);
    auto addedDeckSize = deck->cardCollection.size();
    if (initialDeckSize == addedDeckSize + 1){
        std::cout << "Successfully put a card in the deck. Card added: " << card << ". Original size of the deck: " << initialDeckSize << ", and size after card added: " << addedDeckSize <<std::endl;
    }
    else{
        std::cout << "Error adding a card to the deck. Size of the deck before and after being added is not valid" << std::endl;
    }
}

void CardsTester::testDeckToString() {
    std::vector<Card*> cards = {new BombCard(), new AirliftCard(), new BombCard(), new NegotiateCard, new BlockadeCard};
    auto* deck = new Deck(cards);
    std::cout << deck << std::endl;
    std::cout << "Finished printing Deck" << std::endl;
}

void CardsTester::testHandToString() {
    auto* bombCardOne = new BombCard();
    auto* airliftCardOne = new AirliftCard();
    auto* negotiateCardOne = new NegotiateCard();
    auto* blockadeCardOne = new BlockadeCard();
    auto* hand = new Hand();
    hand->add(bombCardOne);
    hand->add(airliftCardOne);
    hand->add(negotiateCardOne);
    hand->add(blockadeCardOne);
    std::cout << hand << std::endl;
    std::cout << "Finished printing Hand" << std::endl;
}

void CardsTester::testBombCardToString() {
    auto* card = new BombCard();
    std::cout << card;
    std::cout << "Finished printing BombCard" << std::endl;
}

void CardsTester::testBlockadeCardToString() {
    auto* card = new BlockadeCard();
    std::cout << card;
    std::cout << "Finished printing BlockadeCard" << std::endl;
}

void CardsTester::testAirliftCardToString() {
    auto* card = new AirliftCard();
    std::cout << card;
    std::cout << "Finished printing AirliftCard" << std::endl;

}

void CardsTester::testNegotiateCardToString() {
    auto* card = new NegotiateCard();
    std::cout << card;
    std::cout << "Finished printing NegotiateCard" << std::endl;
}

void CardsTester::testHandAdd() {
    auto* card = new BombCard();
    auto* hand = new Hand();
    int initialDeckSize = hand->cards.size();
    hand->add(card);
    int addedHandSize = hand->cards.size();
    std::cout << hand << std::endl;
    if (initialDeckSize == addedHandSize - 1){
        std::cout << "Successfully put a card in the hand. Card added: " << card << ". Original size of the hand: " << initialDeckSize << ", and size after card added: " << addedHandSize <<std::endl;
    }
    else{
        std::cout << "Error adding a card to the hand. Size of the hand before and after being added is not valid" << std::endl;
    }
}

void CardsTester::testHandRemove() {
    auto* card = new BombCard();
    auto* hand = new Hand();
    hand->add(card);
    hand->add(card);
    auto addedHandSize = hand->cards.size();
    hand->remove(card);
    auto removedHandSize = hand->cards.size();
    std::cout << hand << std::endl;
    if (addedHandSize == removedHandSize + 1){
        std::cout << "Successfully removed a card in the hand. Card removed: " << card << ". Original size of the hand: " << addedHandSize << ", and size after card removed: " << removedHandSize <<std::endl;
    }
    else{
        std::cout << "Error removing a card to the hand. Size of the hand before and after being added is not valid" << std::endl;
    }
}



