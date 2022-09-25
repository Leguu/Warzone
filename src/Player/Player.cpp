#include "Player.h"
#include "../GameEngine/GameEngine.h"
#include <iostream>

/**
 * Play a card from the deck and add it to the deck
 * @param cardName The name of the card
 */
void Player::play(std::string &cardName) {
    auto ge = GameEngine::instance();
    Card* cardToPlay = nullptr;
    int index = -1;
    for (auto &card: this->hand->cards) {
        if(card->name == cardName){
            cardToPlay = card;
            break;
        }
        index = index + 1;
    }
    if (cardToPlay != nullptr){
        cardToPlay->play(this);
        this->hand->remove(index);
        ge->deck->put(cardToPlay);
    }else{
        std::cout << "Error playing Card. No card of this name were found" << std::endl;
    }
}

const std::vector<Territory *> Player::toAttack() {
    return std::vector<Territory *>();
}

const std::vector<Territory *> Player::toDefend() {
    return std::vector<Territory *>();
}

void Player::issueOrder() {

}
