#include "Player.h"
#include "../GameEngine/GameEngine.h"
#include <iostream>

/**
 * Play a card from the deck and add it to the deck
 * @param cardName The name of the card
 */
void Player::play(std::string const &cardName) {
  auto ge = GameEngine::instance();
  auto cardToPlay = Player::findCardByName(cardName);
  if (cardToPlay != nullptr) {
    if (cardToPlay->play(this)) {
      this->hand->remove(cardToPlay);
      ge->deck->put(cardToPlay);
    } else {
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

/**
 * Find a card by its name
 * @param name The name of the card
 * @param indexPointer An index we will change for the parent to indicate which card we are on
 * @return A pointer to the card
 */
Card *Player::findCardByName(std::string name) const {
  for (auto &card : this->hand->cards) {
    for (auto cardName : card->getAliases())
      if (Utils::isEqualLowercase(Utils::trim(cardName), name)) {
        return card;
      }
  }
  return nullptr;
}

/**
 * Get all the enemy territories adjacent to your own
 * @return all the enemy territories adjacent to yours
 */
std::vector<Territory *> Player::getAdjacentEnemyTerritories() {
  std::vector<Territory *> enemyTerritoriesAdjacent;
  for (auto friendlyTerritory : this->ownedTerritories) {
    for (auto adjacentTerritory : friendlyTerritory->getAdjTerritories()) {
      if (adjacentTerritory->getOwner() != this && adjacentTerritory->getOwner() &&
          std::find(enemyTerritoriesAdjacent.begin(), enemyTerritoriesAdjacent.end(), adjacentTerritory) ==
              enemyTerritoriesAdjacent.end()) {
        enemyTerritoriesAdjacent.push_back(adjacentTerritory);
      }
    }
  }
  return enemyTerritoriesAdjacent;
}

/**
 * Function to print to console a player's information
 * @param os The OSStream we will be appending to
 * @param hand  The player we will be examining
 * @return A string containing a player's information
 */
std::ostream &operator<<(std::ostream &os, const Player &player) {
  os << player.name << std::endl;
  return os;
}
void Player::drawFromDeck() const {
  auto ge = GameEngine::instance();

  auto card = ge->deck->draw();
  hand->cards.push_back(card);
}
