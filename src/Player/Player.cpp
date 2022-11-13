#include <iostream>
#include <map>

#include "../GameEngine/GameEngine.h"
#include "Player.h"

/**
 * Find all adjacent enemy territories
 * @return all adjacent enemy territories
 */
std::vector<std::pair<Territory *, Territory *>> Player::toAttack() const {
  auto adjacentEnemies = std::vector<std::pair<Territory *, Territory *>>();
  for (auto t : ownedTerritories) {
	for (auto adj : t->getAdjTerritories()) {
	  if (adj->getOwner() && adj->getOwner() != this) {
		adjacentEnemies.emplace_back(adj, t);
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
  return ownedTerritories;
}

/**
 * Get all the enemy territories adjacent to your own
 * @return all the enemy territories adjacent to yours
 */
vector<Territory *> Player::getAdjacentEnemyTerritories() {
  vector<Territory *> enemyTerritoriesAdjacent;
  for (auto friendlyTerritory : this->ownedTerritories) {
	for (auto adjacentTerritory : friendlyTerritory->getAdjTerritories()) {
	  if (adjacentTerritory->getOwner() != this &&
		  adjacentTerritory->getOwner() &&
		  find(enemyTerritoriesAdjacent.begin(), enemyTerritoriesAdjacent.end(),
			   adjacentTerritory) == enemyTerritoriesAdjacent.end()) {
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
void Player::issueOrder(bool debugMode) {
  // Current Mechanism:
  // ------------------
  // 	1. Keep deploying until zero reinforcements are left. Random number each
  // 	turn.
  //    2. If user has a card in their hand, 20% chance they use it (first one
  //    available to them), otherwise they advance order.
  //    3. If OrdersList reaches at least 5-10 moves (decided randomly), the
  //    player is done issuing orders.
  // NOTE: All orders that are issued follow random moves for now.

  if (this->reinforcementsAfterDeploy > 0) {
	issueDeployOrder(debugMode);
  } else if (!this->cardOrderIssued) {
	issueCardOrder(debugMode);
	this->cardOrderIssued = true;
  } else if (!this->advanceOrderIssued) {
	issueAdvanceOrder(debugMode);
	this->advanceOrderIssued = true;
  }
}

void Player::issueDeployOrder(bool debugMode) {
  Territory *target = Utils::accessRandomElement(toDefend());
  int armies = reinforcementsAfterDeploy == 1 ? 1 : Utils::randomNumberInRange(1, reinforcementsAfterDeploy);

  if (debugMode)
	cout << "Issued Deploy Order: " << armies
		 << " units to " + target->getContinent()->getName() << endl;

  orders->push(new DeployOrder(this, armies, target));
  reinforcementsAfterDeploy -= armies;
}

void Player::issueCardOrder(bool debugMode) {
  auto randomCardName = this->hand->cards[0]->name;

  debugMode ? this->hand->debugPlay(randomCardName)
			: this->hand->play(randomCardName);

  std::map<std::string, int> cardNameMap = {
	  {"Bomb", 0}, {"Blockade", 1}, {"Airlift", 2}, {"NegotiateCard", 3}};

  int cardIndex = (cardNameMap.count(randomCardName) > 0)
				  ? (*cardNameMap.find(randomCardName)).second
				  : -1;

  switch (cardIndex) {
  case 0: {
	std::pair<Territory *, Territory *> attack = Utils::accessRandomPair(toAttack());
	orders->push(new BombOrder(this, attack.first));

	if (debugMode)
	  cout << "Issued BombOrder on: " << attack.first->getName() << endl;

	break;
  }

  case 1: {
	Territory *target = Utils::accessRandomElement(this->ownedTerritories);
	orders->push(new BlockadeOrder(this, target));

	if (debugMode)
	  cout << "Issued BlockadeOrder on: " << target->getName() << endl;

	break;
  }

  case 2: {
	Territory *source = nullptr;
	Territory *target = Utils::accessRandomElement(this->ownedTerritories);

	for (auto t : this->ownedTerritories) {
	  if (t->getArmies() > 0) {
		source = t;
	  }
	}

	if (source == nullptr) {
	  return;
	}

	auto armies = source->getArmies();

	if (armies != 1) {
	  std::random_device rd;
	  std::mt19937 rng(rd());
	  std::uniform_int_distribution<int> uni(1, armies);
	}

	orders->push(new AirliftOrder(this, armies, source, target));

	if (debugMode)
	  cout << "Issued AirliftOrder " << armies
		   << " units from: " << source->getName() << " to "
		   << target->getName() << endl;

	break;
  }

  case 3: {
	auto ge = GameEngine::instance();

	Player *randomPlayer;
	do {
	  randomPlayer = Utils::accessRandomElement(ge->players);
	} while (randomPlayer == this);

	orders->push(new NegotiateOrder(this, randomPlayer));

	if (debugMode)
	  cout << "Issued NegotiateOrder by " << this->name
		   << " against: " << randomPlayer->name << endl;

	break;
  }

  default:throw InvalidCardException(randomCardName + " is not a legal card.");
  }
}

void Player::issueAdvanceOrder(bool debugMode) {
  // attack if option available
  Territory *source = nullptr, *target = nullptr;
  auto targetTerritories = toAttack();
  std::pair<Territory *, Territory *> randomPair;
  bool foundPair = false;
  for (auto t : targetTerritories) {
	randomPair = t;
	if (randomPair.second->getArmies() > 0) {
	  target = randomPair.first;
	  source = randomPair.second;
	  foundPair = true;
	  break;
	}
  }

  if (!foundPair) {
	target = Utils::accessRandomElement(toDefend());
	for (auto t : toDefend()) {
	  if (t != target && t->getArmies() > 0) {
		source = t;
	  }
	}
  }

  if (source == nullptr) {
	return;
  }

  int armies = source->getArmies();
  if (armies > 1) {
	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_int_distribution<int> uni(1, armies);
	armies = uni(rng);
  }

  orders->push(new AdvanceOrder(this, armies, source, target));

  if (debugMode)
	cout << "Issued Advance Order: " << armies << " units from "
		 << source->getName() << " [armies = " << source->getArmies() << "] to " << target->getName() << " [armies = "
		 << target->getArmies() << "]" << endl;
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

/**
 * Exception for invalid card
 * @param arg The text that will be printed on error
 */
InvalidCardException::InvalidCardException(const std::string &arg)
	: runtime_error(arg) {}
