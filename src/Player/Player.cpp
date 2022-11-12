#include <iostream>
#include <map>

#include "../GameEngine/GameEngine.h"
#include "Player.h"

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

vector<Territory *> Player::toDefend() const { return ownedTerritories; }

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
bool Player::issueOrder(bool debugMode) {
  // Current Mechanism:
  // ------------------
  // 	1. Keep deploying until zero reinforcements are left. Random number each
  // 	turn.
  //    2. If user has a card in their hand, 30% chance they use it (first one
  //    available to them), otherwise they advance order.
  //    3. If OrdersList reaches at least 5-10 moves (decided randomly), the
  //    player is done issuing orders.
  // NOTE: All orders that are issued follow random moves for now.

  // (1)
  if (this->reinforcementsAfterDeploy > 0) {
	issueDeployOrder(debugMode);
	return false;
  }

  // (2)
  if (!this->hand->cards.empty() && Utils::weightedBoolean(30)) {
	issueCardOrder(debugMode);
  } else {
	issueAdvanceOrder(debugMode);
  }

  // (3)
  return this->orders->getOrdersSize() >= Utils::randomNumberInRange(5, 10);
}

void Player::issueDeployOrder(bool debugMode) {
  Territory *target = Utils::accessRandomElement(toDefend());
  int armies =
	  (reinforcementsAfterDeploy == 1 ? 0
									  : rand() % reinforcementsAfterDeploy) +
		  1;
  target->reinforcementsAdded += armies;
  if (debugMode)
	cout << "Issued Deploy Order: " << armies
		 << " units to " + target->getName() << endl;
  orders->push(new DeployOrder(this, armies, target));
  this->reinforcementsAfterDeploy -= armies;
}

void Player::issueCardOrder(bool debugMode) {
  auto randomCardName = this->hand->cards[0]->name;
  debugMode ? this->hand->debugPlay(randomCardName)
			: this->hand->play(randomCardName);

  Territory *target = Utils::accessRandomElement(this->ownedTerritories);
  std::map<std::string, int> cardNameMap = {
	  {"Bomb", 0}, {"Blockade", 1}, {"Airlift", 2}, {"Negotiate", 3}};

  int cardIndex = (cardNameMap.count(randomCardName) > 0)
				  ? (*cardNameMap.find(randomCardName)).second
				  : -1;

  switch (cardIndex) {
  case 0: {
	Territory *attack = Utils::accessRandomElement(toAttack());
	orders->push(new BombOrder(this, attack));
	if (debugMode)
	  cout << "Issued BombOrder on: " << target->getName() << endl;
	break;
  }

  case 1: {
	orders->push(new BlockadeOrder(this, target));
	if (debugMode)
	  cout << "Issued BlockadeOrder on: " << target->getName() << endl;
	break;
  }

  case 2: {
	Territory *source = Utils::accessRandomElement(this->ownedTerritories);
	while (source->reinforcementsAdded == 0) {
	  source = Utils::accessRandomElement(this->ownedTerritories);
	}
	auto armies = source->reinforcementsAdded;
	armies = Utils::randomNumberInRange(1, armies);
	orders->push(new AirliftOrder(this, armies, source, target));
	source->reinforcementsAdded -= armies;
	target->reinforcementsAdded += armies;
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
	} while (randomPlayer != this);

	orders->push(new NegotiateOrder(this, randomPlayer));
	if (debugMode)
	  cout << "Issued NegotiateOrder by " << this->name
		   << "against: " << randomPlayer->name << endl;
	break;
  }

  default:throw InvalidCardException(randomCardName + " is not a legal card.");
  }
}

void Player::issueAdvanceOrder(bool debugMode) {
  Territory *source = Utils::accessRandomElement(this->ownedTerritories);

  vector<Territory *> territories =
	  Utils::weightedBoolean(50) ? toDefend() : toAttack();
  Territory *target = Utils::accessRandomElement(territories);

  int armies;
  while (source->reinforcementsAdded == 0) {
	source = Utils::accessRandomElement(this->ownedTerritories);
  }
  armies = rand() % source->reinforcementsAdded + 1;

  orders->push(new AdvanceOrder(this, armies, source, target));
  source->reinforcementsAdded -= armies;
  target->reinforcementsAdded += armies;

  if (debugMode)
	cout << "Issued Advance Order: " << armies << " units from "
		 << source->getName() << " to " << target->getName() << endl;
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
