#include "Player.h"
#include "../GameEngine/GameEngine.h"
#include <iostream>

/**
 * Find all adjacent enemy territories
 * @return all adjacent enemy territories
 */
vector<Territory *> Player::toAttack() const {
  auto adjacentEnemies = vector<Territory *>();
  for (auto t : ownedTerritories) {
    for (auto adj : t->getAdjTerritories()) {
      if (adj->getOwner() && adj->getOwner() != this) {
        adjacentEnemies.push_back(adj);
      }
    }
  }
  return adjacentEnemies;
}

/**
 * Find all territories the player owns
 * @return all territories the player owns
 */

vector<Territory *> Player::toDefend() const {
  auto sensitiveTerritories = vector<Territory *>();
  for (auto t : ownedTerritories) {
    for (auto adj : t->getAdjTerritories()) {
      if (adj->getOwner() && adj->getOwner() != this) {
        sensitiveTerritories.push_back(t);
        break;
      }
    }
  }
  return sensitiveTerritories;
}

/**
 * Get all the enemy territories adjacent to your own
 * @return all the enemy territories adjacent to yours
 */
vector<Territory *> Player::getAdjacentEnemyTerritories() {
  vector<Territory *> enemyTerritoriesAdjacent;
  for (auto friendlyTerritory : this->ownedTerritories) {
    for (auto adjacentTerritory : friendlyTerritory->getAdjTerritories()) {
      if (adjacentTerritory->getOwner() != this && adjacentTerritory->getOwner() &&
          find(enemyTerritoriesAdjacent.begin(), enemyTerritoriesAdjacent.end(), adjacentTerritory) ==
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
  os << player.name << endl;
  return os;
}

/**
 * Draw a card from the deck
 */
void Player::drawFromDeck() const {
  auto ge = GameEngine::instance();

  auto card = ge->deck->draw();
  hand->cards.push_back(card);
}

/**
 * Issue all your orders while its your turn
 */
void Player::issueOrder() {
  // --------- Deploy Order ---------
  // keep deploying until zero reinforcements
  int reinforcementsAfterDeploy = this->reinforcements;
  while (reinforcementsAfterDeploy > 0) {
    auto territories = toDefend();

    int randomIndex = rand() % territories.size();
    Territory *target = territories[randomIndex];

    int armies = rand() % reinforcementsAfterDeploy;
    orders->push(new DeployOrder(this, armies, target));

    reinforcementsAfterDeploy -= armies;
  }

  // --------- Advance Order ---------
  // move army units from one of its own territories to..
  int randomSourceIndex = rand() % ownedTerritories.size();
  Territory *source = ownedTerritories[randomSourceIndex];

  vector<Territory *> territories;

  // randomly decide target territory
  auto randomBoolean = rand() > (RAND_MAX / 2);
  if (randomBoolean) {
    territories = toDefend();
  } else {
    territories = toAttack();
  }

  int randomTargetIndex = rand() % territories.size();
  Territory *target = territories[randomTargetIndex];

  int armies = rand() % source->getArmies();
  orders->push(new AdvanceOrder(this, armies, source, target));

  // if cards > 0 and random bool
  //    issueCardOrder();
}

/**
 * Player constructor
 * @param name The name of the player
 */
Player::Player(string name) : name(std::move(name)), orders(new OrderList()) {
  this->hand = new Hand(this);
}
/**
 * Play destructor
 */
Player::~Player() = default;
