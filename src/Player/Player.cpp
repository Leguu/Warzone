#include "Player.h"
#include "../GameEngine/GameEngine.h"
#include <iostream>
#include <random>

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
bool Player::issueOrder() {
  // Current Mechanism:
  // ------------------
  // 	1. Keep deploying until zero reinforcements are left. Random number each turn.
  //    2. If user has a card in their hand, 30% chance they use it (first one available to them), otherwise they advance order.
  //    3. If OrdersList reaches at least 5-10 moves (decided randomly), the player is done issuing orders.
  // NOTE: All orders that are issued follow random moves for now.

  // (1)
  // --------- Deploy Order ---------
  if (this->reinforcementsAfterDeploy > 0) {
	auto territories = toDefend();

	int randomIndex = rand() % territories.size();
	Territory *target = territories[randomIndex];

	int armies = rand() % reinforcementsAfterDeploy;
	orders->push(new DeployOrder(this, armies, target));
	this->reinforcementsAfterDeploy -= armies;

	return false;
  }

  // (2)
  auto thirtyPercentBoolean = (rand() * 1.0f / RAND_MAX) < (0.3f);
  if (!this->hand->cards.empty() && thirtyPercentBoolean) {
	// --------- Card Order ---------
  } else {
	// --------- Advance Order ---------
	int randomSourceIndex = rand() % this->ownedTerritories.size();
	Territory *source = this->ownedTerritories[randomSourceIndex];

	vector<Territory *> territories;
	auto randomBoolean = rand() > (RAND_MAX / 2);
	territories = randomBoolean ? toDefend() : toAttack();

	int randomTargetIndex = rand() % territories.size();
	Territory *target = territories[randomTargetIndex];

	int armies = rand() % source->getArmies();
	orders->push(new AdvanceOrder(this, armies, source, target));
  }

  // (3)
  std::default_random_engine generator;
  std::uniform_int_distribution<int> distribution(5, 7);

  return this->orders->getOrdersSize() >= distribution(generator);
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
