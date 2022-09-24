#include "Player.h"
#include "../GameEngine/GameEngine.h"
#include <iostream>

/**
 * Play a card from the deck and add it to the deck
 * @param name The name of the card
 */
void Player::play(std::string &name) {
    auto ge = GameEngine::instance();
    Card* cardToPlay = nullptr;
    for (auto &card: this->hand->cards) {
        if(card->name == name){
            cardToPlay = card;
            break;
        }
    }
    if (cardToPlay != nullptr){
        if(cardToPlay->name == "AirliftCard" || cardToPlay->name == "NegotiateCard"){
            cardToPlay->play(this);
        }
        ge->deck->put(cardToPlay);
        this->hand->cards.push_back(cardToPlay);
    }else{
        std::cout << "Error playing Card. No card of this name were found" << std::endl;
    }
}


/**
 * Remove a card from a player's hand
 * @param name  The name of the card
 * @return 1 if removed correctly, 0 if not removed
 */
int Player::remove(const std::string& name){
    auto ge = GameEngine::instance();
    for(auto &card : this->hand->cards){
        if (card->name == name){
            ge->deck->put(card);
            return 1;
        }
    }
    return 0;
}

/**
 * Draw a card from the deck and add it to your hand
 */
void Player::draw() {
    auto ge = GameEngine::instance();
    auto card = ge->deck->draw();
    this->hand->cards.push_back(card);
}