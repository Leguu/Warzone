#include "Player.h"
#include "../GameEngine/GameEngine.h"
#include <iostream>

/**
 * Play a card from the deck and add it to the deck
 * @param cardName The name of the card
 */
void Player::play(std::string const &cardName) {
    auto ge = GameEngine::instance();
    int index = 0;
    int *indexPointer = &index;
    Card *cardToPlay = Player::findCardByName(cardName, indexPointer);
    if (cardToPlay != nullptr) {
        if (cardToPlay->play(this)){
            this->hand->remove(index);
            ge->deck->put(cardToPlay);
        } else{
            std::cout << "Cancelled play action" << std::endl;
        }
    } else {
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

Card *Player::findCardByName(std::string name, int *indexPointer) const {
    for (auto &card: this->hand->cards) {
        if (card->name == name) {
            return card;
        }
        indexPointer = indexPointer + 1;
    }
    return nullptr;
}

/**
 * Get all the enemy territories adjacent to your own
 * @return all the enemy territories adjacent to yours
 */
const std::vector<Territory *> Player::getAdjacentEnemyTerritories() {
    std::vector<Territory *> enemyTerritoriesAdjacent;
    for (auto *friendlyTerritory: this->ownedTerritories) {
        for (auto *adjacentTerritory: friendlyTerritory->adjacentTerritories) {
            if (adjacentTerritory->getOwner() != this &&
                std::find(enemyTerritoriesAdjacent.begin(), enemyTerritoriesAdjacent.end(), adjacentTerritory) ==
                enemyTerritoriesAdjacent.end()) {
                enemyTerritoriesAdjacent.push_back(adjacentTerritory);
            }
        }
    }
    return enemyTerritoriesAdjacent;
}
