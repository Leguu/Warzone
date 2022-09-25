//
// Created by danie on 2022-09-24.
//

#include "CardsDriver.h"
#include "./Card.h"
#include "../Player/Player.h"
#include <iostream>


void testCards() {
    std::vector<Card*> cards = {new BombCard(), new AirliftCard(), new BombCard(), new NegotiateCard,new NegotiateCard, new BlockadeCard, new BlockadeCard};
    auto* deck = new Deck(cards);
    auto* hand = new Hand();
    auto* player = new Player("bob");
    while(deck->cardCollection.size() > 0){
        hand->add(deck->draw());
    }
    for(auto* card: hand->cards){
        card->play(player);
    }
    std::cout << "Content of Deck after every card has been played: " << deck << std::endl;
    std::cout << "Content of the hand after card has been played: " << hand << std::endl;
}